#include <cstring>

#include "Reflex/Member.h"
#include "Reflex/Object.h"

#include "CondCore/DBCommon/interface/Exception.h"
#include "PrimitivesContainerStreamer.h"

#include "RVersion.h"

#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
using namespace ROOT;
#endif

cond::BlobWriter::BlobWriter( const TypeH& type ):
  m_type( type ),
  m_blob()
{}

cond::BlobWriter::~BlobWriter()
{}

const coral::Blob&
cond::BlobWriter::write( const void* addressOfInputData )
{
  // std::cout<<"cond::BlobReader::write"<<std::endl;
  Reflex::Object theContainer( m_type, const_cast<void*>( addressOfInputData ) );
  //get the container size
  Reflex::Member sizeMethod = m_type.MemberByName( "size" );
  if ( ! sizeMethod )
    throw cond::Exception( "BlobWriter::write No size method is defined for the container" );
  size_t containerSize=0;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
  ROOT::Reflex::Object ret = sizeMethod.Invoke(theContainer);
  if( !ret.TypeOf() || !ret.Address() )
    throw cond::Exception( "BlobWriter::write Could not invoke the size method on the container" );
  containerSize = *(static_cast<size_t*>(ret.Address()));
#else
  Reflex::Object* ret=0;
  sizeMethod.Invoke(theContainer,ret);
  if( !ret->TypeOf() || !ret->Address() )
    throw cond::Exception( "BlobWriter::write Could not invoke the size method on the container" );
  containerSize = *(static_cast<size_t*>(ret->Address()));
#endif
 
  if(containerSize==0){
    //std::cout<<"empty container nothing to write"<<std::endl;
    return m_blob;
  }
  // std::cout<<"container size "<<containerSize<<std::endl;
  //get the element size
  //size_t nargs=m_type.TemplateArgumentSize();
  //std::cout<<"template arguments to vector "<<nargs<<std::endl;
  size_t elementSize=m_type.TemplateArgumentAt(0).SizeOf();
  //std::cout<<"elementSize "<<elementSize<<std::endl;
  Reflex::Member beginMethod = m_type.MemberByName( "begin" );
  if ( ! beginMethod )
    throw cond::Exception( "BlobWriter::write No begin method is defined for the container" );
  TypeH iteratorType = beginMethod.TypeOf().ReturnType();
  Reflex::Member dereferenceMethod = iteratorType.MemberByName( "operator*" );
  if ( ! dereferenceMethod )
    throw cond::Exception( "BlobWriter::write Could not retrieve the dereference method of the container's iterator" );
  //size_t elementSize = dereferenceMethod.TypeOf().ReturnType().SizeOf();
  // Create an iterator
  void* elementAddress=0;
  void* startingAddress=0;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
  ROOT::Reflex::Object iteratorObject = beginMethod.Invoke( ROOT::Reflex::Object( m_type, const_cast< void * > ( addressOfInputData ) ) );
  elementAddress = dereferenceMethod.Invoke( iteratorObject ).Address();
  m_blob.resize( containerSize * elementSize );
  startingAddress = m_blob.startingAddress();
  ::memcpy( startingAddress, elementAddress, containerSize*elementSize );
  iteratorObject.Destruct();
#else
  Reflex::Object* containercontents=0;
  Reflex::Object* iteratorObject=0;
  beginMethod.Invoke( ROOT::Reflex::Object( m_type, const_cast< void * > ( addressOfInputData ) ),iteratorObject );
  if( !iteratorObject ) throw cond::Exception( "BlobWriter::write Could not retrieve the iterator object" );
  dereferenceMethod.Invoke( *iteratorObject, containercontents);
  if ( ! containercontents )
    throw cond::Exception( "BlobWriter::write Could not retrieve the content of the container" );
  elementAddress=containercontents->Address();
  m_blob.resize( containerSize * elementSize );
  startingAddress = m_blob.startingAddress();
  ::memcpy( startingAddress, elementAddress, containerSize*elementSize );
  iteratorObject->Destruct();
#endif
  return m_blob;
}

cond::BlobReader::BlobReader( const TypeH& type ):
  m_type( type )
{}


cond::BlobReader::~BlobReader()
{}


void
cond::BlobReader::read( const coral::Blob& blobData,
                        void* containerAddress ) const
{
  //std::cout<<"cond::BlobReader::read"<<std::endl;
  const void * srcstartingAddress=blobData.startingAddress();
  //std::cout<<"srcstartingAddress "<<srcstartingAddress<<std::endl;
  //std::cout<<"dest Address "<<containerAddress<<std::endl;
  long bsize=blobData.size();
  if(bsize==0){
    //std::cout<<"empty blob to read"<<std::endl;
    return;
  }
  //std::cout<<"blob size "<<bsize<<std::endl;
  Reflex::Member clearMethod = m_type.MemberByName( "clear" );
  if ( ! clearMethod )
    throw cond::Exception( "BlobReader::read Could not retrieve the clear method of the container" );
  Reflex::Object containerObject( m_type, containerAddress );
  // Clear the container
  clearMethod.Invoke( containerObject );
  //const Reflex::Type signature();
  Reflex::Member resizeMethod = m_type.MemberByName( "resize",TypeH::ByName("void (size_t)") );
  if ( ! resizeMethod )
    throw cond::Exception( "BlobReader::read Could not retrieve the resize method of the container" );
  // resize the container
  //size_t nargs=m_type.TemplateArgumentSize();
  //std::cout<<"template arguments to vector "<<nargs<<std::endl;
  size_t elementSize=m_type.TemplateArgumentAt(0).SizeOf();
  //std::cout<<"elementSize "<<elementSize<<std::endl;
  std::vector<void *> v(1);
  size_t containerSize = bsize / elementSize;
  //std::cout<<" container size "<<containerSize<<std::endl;
  //std::cout<<" container size address "<<(void*)(&containerSize)<<std::endl;
  /*for(size_t i=0; i<containerSize; ++i){
    containerObject.Invoke("push_back", 0); //hack!!
  }
  */
  v[0] = (void*)(&containerSize);
  //int temp=0;
  //v[1] = (void*)(&temp);
  //v.push_back((void*)(&containerSize));
  //std::cout<<resizeMethod.Name()<<std::endl;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
  resizeMethod.Invoke( containerObject,v );
  ROOT::Reflex::Member beginMethod = m_type.MemberByName( "begin" );
  if ( ! beginMethod )
    throw cond::Exception( "BlobReader::read No begin method is defined for the container" );
  ROOT::Reflex::Object iteratorObject = beginMethod.Invoke( ROOT::Reflex::Object( m_type, const_cast< void * > ( containerAddress ) ) );
  ROOT::Reflex::Type iteratorType = beginMethod.TypeOf().ReturnType();
  //get first element address
  ROOT::Reflex::Member dereferenceMethod = iteratorType.MemberByName( "operator*" );
  if ( ! dereferenceMethod )
    throw cond::Exception( "BlobReader::read Could not retrieve the dereference method of the container's iterator" );
  void* elementAddress = dereferenceMethod.Invoke( iteratorObject ).Address();
  ::memcpy( elementAddress, srcstartingAddress, (size_t)bsize);
  iteratorObject.Destruct();
#else
  Reflex::Object* m=0;
  resizeMethod.Invoke( containerObject,m,v );
  // Create an iterator
  Reflex::Member beginMethod = m_type.MemberByName( "begin" );
  if ( ! beginMethod )
    throw cond::Exception( "BlobReader::read No begin method is defined for the container" );
  Reflex::Object* iteratorObject=0; 
  beginMethod.Invoke( Reflex::Object( m_type, const_cast< void * > ( containerAddress ) ),iteratorObject );
  if ( ! iteratorObject )
    throw cond::Exception( "BlobReader::read Could not retrieve the iterator of the container" );
  Reflex::Type iteratorType = beginMethod.TypeOf().ReturnType();
  //get first element address
  Reflex::Member dereferenceMethod = iteratorType.MemberByName( "operator*" );
  if ( ! dereferenceMethod )
    throw cond::Exception( "BlobReader::read Could not retrieve the dereference method of the container's iterator" );
  void* elementAddress = 0;
  Reflex::Object* ret=0;
  dereferenceMethod.Invoke( *iteratorObject, ret );
  if ( ! ret )
    throw cond::Exception( "BlobReader::read Could not retrieve the content of the container" );
  ret->Address();
  ::memcpy( elementAddress, srcstartingAddress, (size_t)bsize);
  iteratorObject->Destruct();
#endif
}
