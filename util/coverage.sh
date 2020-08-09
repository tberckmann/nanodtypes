
# Perform code coverage check

# Usage: sh util/coverage.sh 95.0

gcov=${1-gcov}
covdir=${2-coverage}
threshold_pct=${3-93} # Default coverage percentage if not passed on command line
final_result=0

echo "Code coverage summary"

if ! test -d coverage
then
    mkdir coverage
fi

for cov_file in `ls obj/*.gcda`
do
    if ! $gcov -k -m $cov_file | awk '
        /^File/ {
            sub(/[^\/]*\//, "") # Strip up to and including forward slash
            sub(/'"'"'/,"") # Strip single quote
            filename = $0
        }
        /^Lines/ {
            match($0, /[0-9]*\.[0-9]*/)
            pct = substr($0, RSTART, RLENGTH)
        }
        END {
            printf("%-15s %.1f%%\n", filename, pct)
            if(pct < '$threshold_pct')
                exit 1
        }
    '
    then
        final_result=1 # Threshold not met
    fi
done
mv *.gcov $covdir

exit $final_result