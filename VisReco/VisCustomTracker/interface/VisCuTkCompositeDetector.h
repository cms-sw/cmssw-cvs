#ifndef VIS_CU_TK_COMPOSITE_DETECTOR_H
#define VIS_CU_TK_COMPOSITE_DETECTOR_H

#include <vector>


template<class T> 
class VisCuTkCompositeDetector {
	public:
           VisCuTkCompositeDetector(); 
	   virtual ~VisCuTkCompositeDetector();
	   unsigned int components() const ;
	   void append(T * det);  
	   void addComponent(unsigned int n, T * det); 
	   T * getComponent(unsigned int n) const;
	   std::vector<T*> allComponents() const;
	private:
	   std::vector<T*> theComponents;
	   unsigned int theSize;
};
#include "VisCuTkCompositeDetector.icc"
#endif

