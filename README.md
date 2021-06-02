# My-Tar

SYNOPSIS 
mytar[cxtf:] [file]filename

DESCRIPTION
mytar creates an archive from of a directory tree, extracts files from an archive, or prints the contents and details of an archive file. Options followed by a ‘:’ expect a subsequent parameter. The options are as follows:
-c
create an archive of the given directory tree. A directory name must be specified.

-x
extract the directory tree contained in the specified archive

-t
print the contents of the specified archive

-f:
the subsequent argument is the name of the archive file (to create, extract or print). This option must always be specified.

EXIT STATUS
mytar exits 0 on success and -1 on failure.
