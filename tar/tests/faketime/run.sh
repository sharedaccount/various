rm -f test.tar 
# make && 
echo "RUN" && ../../src/tar --owner root --group root  --numeric-owner --faketime "2013-01-01 01:00:01" -c -f test.tar EXAMPLEDIR && tar -t -v -f test.tar
