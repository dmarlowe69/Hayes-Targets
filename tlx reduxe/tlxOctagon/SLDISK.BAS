10 .. date:  2-21-90
20 .. Demonstration program
30 .. Save data to disk using PC SmartLink
40 .. 480 bytes are saved to disk.  Data is CHR$(N), which is for/next data
50 ..
60 .. open file for new.  Old data will be replaced.
70 PRINT CHR$(27);"DON";CHR$(0);"test$";
80 FOR X = 0 TO 3
90 ..  save 120 characters to disk buffer
100    PRINT CHR$(27);"DBW";CHR$(120);  :.. 128 characters maximum
110 ..    print data to disk
120    FOR N = 0 TO 119
130       PRINT CHR$(N);    :.. this is the data to save to disk
140    NEXT
150 ..    save data to disk buffer
160    PRINT CHR$(27);"DFW";CHR$(0);CHR$(120);   :.. 128 characters maximum
170 ..    redo next loop of data to save
180 NEXT
190 .. close out file
200 PRINT CHR$(27);"DC";CHR$(0);
210 END
