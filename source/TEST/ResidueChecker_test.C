// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/residueChecker.h>

///////////////////////////

START_TEST(ResidueChecker)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ResidueChecker* rc_ptr = 0;
CHECK(ResidueChecker())
	rc_ptr = new ResidueChecker;
	TEST_NOT_EQUAL(rc_ptr, 0)
RESULT								

CHECK(~ResidueChecker())
	delete rc_ptr;
	rc_ptr = 0;
RESULT

// ????
// Real tests still missing

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
