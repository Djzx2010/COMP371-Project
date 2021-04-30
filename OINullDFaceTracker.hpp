#pragma once

#include "OINullFaceTracker.hpp"

//OpenISS namespace
namespace openiss
{
	//This is our class to track dynamically changing 
	class OINullDFaceTracker : public openiss::OINullFaceTracker {
		
	public:

		//Default constructor
		OINullDFaceTracker();
		
		//Copy Constructor
		OINullDFaceTracker(const OINullDFaceTracker& copied);
		
		//Destructor
		~OINullDFaceTracker();

		//Assignment operator
		OINullDFaceTracker& operator=(OINullDFaceTracker&);

		//Returns the next face in the vector
		OIFace* getNextFace();

		//Method to generates the points for our  faces
		void generateFaces();

		//Accessors
		int getCurrentFace() const;
		vector<OIFace*>* getOIFace() const;

		//Mutators
		void setCurrentFace(int);
		void setOIFace(vector<OIFace*>*);


	private:

		//Current face keeps track of last face sent
		int currentFace = 6;
		
		//Faces is a collection of OIFace
		vector<OIFace*>* oiface;


	};
}