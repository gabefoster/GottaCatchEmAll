# GottaCatchEmAll

GottaCatchEmAll is an example of a basic CMake project with Catch2, CTest, Jenkins, and Doxygen intergration.
It's meant to represent a typical project I might throw together in an afternoon, as all my recent work is proprietary.
The code in this repo was written in one session (though it is based on a somewhat similar project I've worked on before).

## Building the Tests

Simply compile the included CMakeLists.txt with `-DBUILD\_TESTING=true`.
To compile only tests with specific tags, add `-DTEST\_TAGS="[tag1][tag2]"`.

## Running the Tests

After compilation, run ctest.

## Jenkins Integration

GottaCatchEmAll outputs logs in junit format, which can be easily parsed and displayed in Jenkins.

## Doxygen Integration.

Calling doxygen on the included Doxfile (from the docs directory) will generate documentation for GottaCatchEmAll.

Enjoy!
