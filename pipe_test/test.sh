ps -s > base
wc -l base | tr -dc '0-9' | cat > std
ps -s > tested
wc -l tested | tr -dc '0-9' | cat > cur
DIFF=$(diff std cur)
echo $DIFF
while [ 1 ]
do
ps -s > tested
wc -l tested | tr -dc '0-9' | cat > cur
DIFF=$(diff std cur)
if [ "$DIFF" != "" ]
then
cat tested
fi
done
