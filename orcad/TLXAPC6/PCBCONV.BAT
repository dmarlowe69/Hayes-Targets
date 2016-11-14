Echo Off
Set ORCADEXE=C:\ORCAD\ORCADEXE\
If not %ORCADEXE%. == C:\ORCAD\ORCADEXE\. Goto ENVERR
Set ORCADESP=C:\ORCAD\ORCADESP\
If not %ORCADESP%. == C:\ORCAD\ORCADESP\. Goto ENVERR
Set ORCADUSER=C:\ORCAD\ORCADESP\
If not %ORCADUSER%. == C:\ORCAD\ORCADESP\. Goto ENVERR
Set ORCADPROJ=C:\ORCAD\ORCAD\
If not %ORCADPROJ%. == C:\ORCAD\ORCAD\. Goto ENVERR
Set OldPath=%PATH%
If not %OldPath%. == %PATH%. Goto ENVERR
Set PATH=%ORCADEXE%
If not %PATH%. == %ORCADEXE%. Goto ENVERR
Cls
C:
Cd C:\ORCAD\ORCAD\TEMPLATE
Echo   You have opted to have the ORCADESP.DAT files in each of your
Echo   project directories automatically updated.  You have also elected
Echo   to automatically convert boards in each of your design directories and
Echo   module files to their PCB 386+ v1.00 equivalents.  The converted
Echo   designs will be stored in the same directories as the originals but
Echo   will use a board file extension of ".BD1".
Echo.
Echo   Please note that you must manually update any ORCADESP.DAT, board,
Echo   and module files located in additional design directories that are
Echo   not located under
Echo.
Echo       %ORCADPROJ%
Echo.
Echo   If you wish to abort this process now, press Ctrl-C.  You will
Echo   need to manually update the ORCADESP.DAT files and convert the
Echo   ORCAD/PCB II board and module files before using PCB 386+.
Echo.
Echo.
Pause
Cls
Echo   For instructions on manual updates and conversion of the following
Echo   files, please see the corresponding documentation included with
Echo   this release:
Echo.
Echo   ORCADESP.DAT............OrCAD Technical Note #45
Echo.
Echo   PCB II board files......Fast Track
Echo   and library files
Echo.
Echo.
Pause
Cls
Echo.
Echo.
Echo   After the board files are converted, this procedure will update the
Echo   ORCADESP.DAT file in
Echo.
Echo       C:\ORCAD\ORCAD\TEMPLATE\
Echo.
Echo   Then this new ORCADESP.DAT will be used to update each of the other
Echo   ORCADESP.DAT files in design directories located under
Echo.
Echo       C:\ORCAD\ORCAD\
Echo.
Echo   This conversion and update process will begin now.
Echo.
Echo.
Pause
If not exist ORCADESP.DAT set NoDatFile=TRUE
If not exist C:\ORCAD\ORCADESP\PCB386.MRG set NoMrgFile=TRUE
If not exist C:\ORCAD\ORCADEXE\FROMPCB2.EXE set NoBrdConv=TRUE
If not exist C:\ORCAD\ORCADEXE\MOD2MLB.EXE set NoModConv=TRUE
If %NoDatFile%. == TRUE. goto FILEERR
If %NoMrgFile%. == TRUE. goto FILEERR
If %NoBrdConv%. == TRUE. goto FILEERR
If %NoModConv%. == TRUE. goto FILEERR
Cls
Echo.
Echo.
Echo     Converting PCB II boards to the PCB 386+ format.
Echo.
C:\ORCAD\ORCADEXE\FROMPCB2 *.BRD /Ig /R /S C:\ORCAD\ORCAD
If errorlevel 5 goto CONVERR
Cls
Echo.
Echo.
Echo     Converting PCB II module files to the equivalent PCB 386+ module libraries.
Echo.
Cd \ORCAD\ORCADESP\PCB\LIBRARY
MOD2MLB CUSTOM1.MLB C:\ORCAD\ORCADESP\PCB\MODULE\*. /Tar . /Com /Ign
If errorlevel 5 goto CONVERR
Cd \ORCAD\ORCAD\TEMPLATE
Cls
Echo.
Echo.
Echo     Updating ORCADESP.DAT files in project directories...
Echo.
Copy C:\ORCAD\ORCADESP\PCB386.MRG C:\ORCAD\ORCADESP\ORCADESP.MRG
Cd C:\ORCAD\ORCAD\TEMPLATE
Set ORCADPROJ=C:\ORCAD\ORCAD\
If not %ORCADPROJ%. == C:\ORCAD\ORCAD\. Goto ENVERR
C:\ORCAD\ORCADEXE\MERGEDAT C:\ORCAD\ORCADESP\ORCADESP.MRG /A
If errorlevel 1 goto MERGERR
If not exist C:\ORCAD\ORCADESP\ORCADESP.MRG goto NOMRGF
Del C:\ORCAD\ORCADESP\ORCADESP.MRG
:NOMRGF
Goto SUCCESS
:FILEERR
Cls
Echo  ERROR:  Unable to find the following file(s).
Echo.
If %NoMrgFile%. == TRUE. echo          C:\ORCAD\ORCADESP\PCB386.MRG
If %NoDatFile%. == TRUE. echo          C:\ORCAD\ORCAD\TEMPLATE\ORCADESP.DAT
If %NoModConv%. == TRUE. echo          C:\ORCAD\ORCADEXE\MOD2MLB.EXE
If %NoBrdConv%. == TRUE. echo          C:\ORCAD\ORCADEXE\FROMPCB2.EXE
Echo.
Echo          The files listed above are critical for this update process to
Echo          complete.
:MRG
If not %NoMrgFile%. == TRUE. goto DAT
Echo.
Echo          The PCB386.MRG file is used to update the screen buttons, PCB 386+
Echo          executable references, and PCB 386+ local configurations for ESP.
:DAT
If not %NoDatFile%. == TRUE. goto MODCONV
Echo.
Echo          The ORCADESP.DAT file may have been missed if
Echo.
Echo              C:\ORCAD\ORCAD\TEMPLATE
Echo.
Echo          is not where you normally store your project directories (they may
Echo          be on another drive).  If this is true, then the PCB 386+ v1.00
Echo          installation made a TEMPLATE directory on drive C:, the same
Echo          drive that the PCB 386+ v1.00 executable files were installed to.
Echo          Given these circumstances, you should have chosen to manually update
Echo          your ORCADESP.DAT files.
:MODCONV
If not %NoModConv%. == TRUE. goto BRDCONV
Echo.
Echo          MOD2MLB.EXE is used during the phase of operation devoted to
Echo          converting your individual PCB II module files to equivalent
Echo          PCB 386+ module libraries.  It converts the individual module
Echo          files it encounters and collects them into a single module
Echo          library for PCB 386+.
:BRDCONV
If not %NoBrdConv%. == TRUE. goto CONVMRG
Echo.
Echo          FROMPCB2.EXE is used during the board conversion phase.  It
Echo          performs the conversion of your PCB II boards to their PCB
Echo          386+ equivalents.
Echo.
Pause
:CONVMRG
Echo.
Echo          If the installation process was unable to run this batch file
Echo          due to memory constraints and indicated to run manually, then
Echo          please procede.
Echo.
Echo          If, however, some time has passed since the installation of
Echo          PCB 386+ and you are running this batch file from the DOS
Echo          command prompt, the files indicated as missing earlier may
Echo          have been deleted or moved from their original location.  If
Echo          these are the conditions you are presently operating under,
Echo          it would be safest to perform a standard installation of
Echo          the PCB 386+ v1.00 software from the product disks.  This
Echo          will ensure that other files involved in this process are
Echo          located where they are expected and provide a greater
Echo          likelihood that the update process will be successful.
Echo.
Pause
Goto END
:MERGERR
Pause
Cls
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo     ERROR: MERGEDAT.EXE did not complete successfully.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Pause
Goto EXEERR
:CONVERR
Pause
Cls
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo     ERROR: FROMPCB2.EXE did not complete successfully.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Echo.
Pause
Goto EXEERR
:EXEERR
Cls
Echo.
Echo.
Echo.
Echo.
Echo.
Echo            Please look over the messages displayed on the screen to determine
Echo            where the problem arose.  Then either make the necessary corrections
Echo            and run the program manually, or contact our Technical Support
Echo            Department for assistance.
Echo.
Echo            If you choose to run the program manually, consult the
Echo            documentation accompanying PCB 386+ for instruction.
Echo.
Echo            If you choose to call Technical Support, please have a
Echo            copy of the error information available to aid the
Echo            Support Engineer in determining what caused the
Echo            failure.
Echo.
Echo.
Echo.
Echo.
Pause
Goto END
:ENVERR
Cls
Echo.
Echo.
Echo.
Echo.
Echo     ERROR: It appears that there is not enough environment space to
Echo            temporarily set environment variables required to perform
Echo            the automatic conversion of the files you have indicated.
Echo            You may need to increase your environment size by adding
Echo            or modifying the SHELL statement in your CONFIG.SYS file
Echo            as described in your DOS manual.
Echo.
Echo            You will now need to make sure that the ORCADEXE, ORCADESP,
Echo            ORCADPROJ, and ORCADUSER environment variables are set
Echo            correctly and then run MERGEDAT.EXE according to the
Echo            instructions in the documentation accompanying the PCB 386+
Echo            product.
Echo.
Echo.
Echo.
Echo.
Pause
Goto END
:SUCCESS
Cls
Echo.
Echo.
Echo.
Echo.
Echo                The automated update process has completed.
Echo.
Echo.
Echo      To speed up the process of becoming familiar with PCB 386+, please
Echo      take time to review the information contained in README.1ST,
Echo      README.DOC, and PCB100.DOC text files.  They can be found in
Echo.
Echo          C:\ORCAD\ORCADESP\PCB
Echo.
Echo      If you are using the ESP Design Environment, you may also view them
Echo      using the View Reference Material button accessible from the PC
Echo      Board Layout Tools screen.
Echo.
Goto END
:END
Cd C:\ORCAD\ORCAD\TEMPLATE
Set NoDatFile=
Set NoMrgFile=
Set NoBrdConv=
Set NoModConv=
Set PATH=%ORCADEXE%;%OldPath%
Set OldPath=
Set ORCADEXE=C:\ORCAD\ORCADEXE\
Set ORCADESP=C:\ORCAD\ORCADESP\
Set ORCADUSER=C:\ORCAD\ORCADESP\
Set ORCADPROJ=C:\ORCAD\ORCAD\
