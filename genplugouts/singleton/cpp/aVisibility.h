#ifndef _AVISIBILITY_H
#define _AVISIBILITY_H


// The visibility of a class member
//
// The 'default visibility' is used to configure the Drawing Settings and
// for the C++ cases (see UmlBaseClassMember::CppVisibility())
enum aVisibility {
  PublicVisibility,
  ProtectedVisibility,
  PrivateVisibility,
  DefaultVisibility

};
#endif
