�J                                                                                                                                                                                  
*** Unable to open input file  ! 
*** Input file cannot have .BAS extension!
 
Binary File to CAMBASIC/STD BASIC DATA Statement Conversion Utility V1.0.
 
Input binary file name : 
Output BASIC file name : 
Conversion complete.
 
*** No file name specified!

 Command Format is:
    MAKBDATA filespec [starting line number] [line increment]

 Example: MAKBDATA TEST.BIN will:
 Open the file TEST.BIN, read as binary data,
 convert data into CAMBASIC/STD BASIC DATA statements and
 write out to the file TEST.BAS.

 The starting line number and line increment parameters
 are optional. The default for each is 10. (10,20,30...)
 
*** Starting line number too large! Must be less than 65530.
 
*** Line increment too large! Must be less than 65530.
 
*** Error ... Next computed line number exceedes maximum line number (65529)!
Try changing the starting line number and/or the line increment.
 ��	
 �
 ���� s� �U��6��p� �d����R� =�!rZ��dR�<3��!s�`�Z�� ��?�� ��!rYHu%�> u��r"�&���u��Us��	�N����>��!�>��!����s^�o���i�q���L�!���Y��� 3ɾ� �
�t���< t�N�63�A�< t<t<ar�<zw�l� ����6���D� �6�dQ��YIO�<.t<\t< tO���O�!V���<.u�<Bu�<Au�<Su	���� ^��^�.B��AS��>t� �6�< t�<t�<0r�<9w�  �
 2�P�����Xr,0�r���w�< t!<t<0r<9w�ؾ=�o �þ~���	�T�	�< t�<tC<0r�<9w�  �
 2�P�����Xr�,0�rȃ��wì< t<t
<0r<9w�؃� t
����	+�	�ì
�t�
 ��ð� �
����!ð&�����������0<:r���$0<:r��,�À> t�����á	r�=��w���	�Ȁ&��'�+ ���% �d � �
 � �� � � D��AT��A ���3����u�t0�,0��+Ȁ���O�
��+��ϋ�@�!�