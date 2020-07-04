
if test ${1-""} = --dry-run
then
    dry_run=true
else
    dry_run=false
fi

tmpstem=tmpfile-$$
tmpname=/tmp/$tmpstem

# Remove duplicate blank lines
for fname in `find src test -name '*.[ch]'`
do
    awk '
        $0 != "" || prevline != "" {print}
        {prevline = $0}
    ' $fname > $tmpname
    indent -di32 -ldi20 -sob -nut -i4 $tmpname
    if $dry_run
    then
        if ! cmp -s $tmpname $fname
        then
            rm $tmpstem.BAK
            exit 1  # clean steps would change something
        fi
    else
        mv $tmpname $fname
    fi
done

# remove the backup files created by indent
if $dry_run
then
    rm $tmpname
fi
rm $tmpstem.BAK