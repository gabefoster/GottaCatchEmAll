# GottaCatchEmAll

GottaCatchEmAll is an example of a basic CMake project with Catch2, CTest, Jenkins, and Doxygen intergration.
It's meant to represent a typical project I might throw together in an afternoon, as all my recent work is proprietary.
The code in this repo was written in one session (though it is based on a somewhat similar project I've worked on before).

## Building the Tests

Simply compile the included CMakeLists.txt with `-DBUILD\_TESTING=true`.
To compile only tests with specific tags, add `-DTEST\_TAGS="[tag1][tag2]"`.

## Running the Tests

After compilation, run ctest.

## Adding a test

The test directory structure mirrors the src and include directory structures for a reason.
Each directory corresponds to both a library (on the source code side) and a suite of file-level tests (on the test code side). With this in mind adding a test is as simple as appending an item to the `TEST_LIST` variable in the CMakeLists.txt.
Doing so will cause the generation of a new test executable which both includes and links the necessary directory and library.
This makes adding test executables a breeze.

## Jenkins Integration

GottaCatchEmAll outputs logs in junit format, which can be easily parsed and displayed in Jenkins.

## Doxygen Integration.

Calling doxygen on the included Doxfile (from the docs directory) will generate documentation for GottaCatchEmAll.

Enjoy!
