echo off
rem This is the installation batch file for American Automation's
rem EZ-DEMO (release 89.1)
rem
rem See if we are on the correct drive
    if not exist instdemo.exe goto WRONG_DRIVE

rem Check for the proper parameters
    if "%1" == "" goto NO_ARGUMENTS

rem See if the user specified a drive.  If not, re-run INSTALL for drive C
    if "%2" == "" INSTALL %1 C

rem See if the hard drive is available and has enough room
    CHECKSPC %2 500
    if errorlevel 255 goto NO_DRIVE
    if errorlevel   1 goto NO_SPACE

rem If we get here, we have a destination  drive.  So, see if the specified
rem directory exists

    CHECKDIR %2:%1
    if errorlevel 255 goto INVALID_PATH
    if errorlevel 1 goto no_directory
    if errorlevel 0 goto dir_exists

:no_directory
    echo INSTALL will now attempt to create the directory %2:%1.
    echo Press Control-C to cancel this, or
    pause

    md %2:%1

    rem Verify that creation was successful
    CHECKDIR %2:%1
    if errorlevel 1 goto DIR_NOT_MADE

:dir_exists
    echo ÿ
    echo EZ-DEMO 89.1 will be installed in %2:%1.
    echo If you do not wish to do this, press Control-C now.  Otherwise,
    pause

    if exist %2:%1\*.dbd del %2:%1\*.dbd > NUL:
    instdemo -r %2:%1
    copy read.me %2:%1 > NUL:
    %2:
    cd %1
    echo ÿ
    echo Installation successful -- type EZDEMO to begin EZ-DEMO 89.1
    goto done

rem Error reporting routines
:NO_ARGUMENTS
    echo INSTALL will now place the EZ-DEMO into c:\EZDEMO.  If the
    echo directory does not exist, it will be created.
    echo ÿ
    echo If you wish to stop this, or to specify a different disk or
    echo directory, type Control-C now, then enter the command
    echo                INSTALL directory [drive]
    echo where directory is a directory name (remember the leading '\'
    echo character), and drive is the letter of the drive where this
    echo should be installed.  (The drive letter is optional, and defaults
    echo to 'C'.)
    echo ÿ
    echo If you specify a drive letter, do not include the trailing ":"
    echo ÿ
    echo ÿ
    echo To stop the installation now, press Control-C.  Otherwise
    pause
    INSTALL \EZDEMO C

:WRONG_DRIVE
    echo Please switch over to the floppy drive before running INSTALL
    goto done

:NO_DRIVE
    echo INSTALL was unable to locate the %2 drive.
    echo If the drive specification includes a ":", please try again without
    echo the trailing ":"
    goto done

:NO_SPACE
    echo INSTALL needs at least 500K of free space to install
    goto done

:INVALID_PATH
    echo The specified path name was invalid
    echo (did you put the drive letter in front by mistake?)
    goto done

:DIR_NOT_MADE
    echo INSTALL could not create the specified directory.
    echo Please check your typing and try again

rem Exit the installation
:done


