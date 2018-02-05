[https://ci.appveyor.com/api/projects/status/github/r-darwish/dupfiles-cpp?svg=true](https://ci.appveyor.com/api/projects/status/github/r-darwish/dupfiles-cpp?svg=true)

# Dupfiles
The purpose of this application is to find duplicate files inside a given directory. It does this by
storing file names in buckets of (size, SHA1). Files that have the same SHA1 result and size are
assumed to be identical.

I'm not an expert in hashing and cannot tell the probability of different
files that have the same hash and size. I don't compare the file content in case of a hash match so
use the results of this tool with care. In any case, this tool does not alter your files, it only
suggests which of them are duplicate.

The tool was built for Windows users which are not familiar with the command line. Thus, despite
having no GUI. The resulting application is designed to be dropped in the directory which the user
wishes to check and then invoked by double clicking on it. It will place a text file called
`Duplicate Files.txt` that stores its result.

The tool uses memory map to hash the files, and so it might fail indexing large files when built for
32-bit platforms.

Dependencies are installed using the [Conan](https://www.conan.io/) package manager and the project
is built using CMake.

# LICENSE
GNU General Public License v3.0
