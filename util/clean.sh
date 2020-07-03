
tmpname=/tmp/cleanfile-$$

# Remove duplicate blank lines
for fname in `find src test -name '*.[ch]'`
do
    awk '
        $0 != "" || prevline != "" {print}
        {prevline = $0}
    ' $fname > $tmpname
    mv $tmpname $fname
done

# Run indent on all C source and header files
find src test -name '*.[ch]' | xargs -L 1 indent -di32 -ldi20 -sob -nut -i4

# Remove the backup files created by indent
rm *.BAK