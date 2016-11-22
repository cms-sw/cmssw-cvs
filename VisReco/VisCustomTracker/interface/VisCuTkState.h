#ifndef VisCuTkState_H
#define VisCuTkState_H

class VisCuTkState{
	public:
       	  //return ID number of subdetector part
	  virtual int getId() = 0;
	  //return true if subdetector part is visible
	  virtual bool isVisible() = 0;
	  //change state  
	  virtual void setVisible() = 0;
	  virtual void setVisible(bool vis) = 0;
};

#endif
