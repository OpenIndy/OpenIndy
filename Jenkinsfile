node {
    try {
        def currentWorkSpace = pwd()
		stage('Checkout')
			checkout scm
        stage('compile debug') {
            bat '''call "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat" x86_amd64
            "C:\\Qt\\5.5\\msvc2013_64\\bin\\qmake.exe" openIndy.pro -r -spec win32-msvc2013 "CONFIG+=debug"
            nmake clean
            nmake'''
            bat "echo build openIndy debug finished"
        }
        stage('prepare debug folder'){
            bat (/del "${currentWorkSpace}\bin\debug\oisystemdb.sqlite"
            del "${currentWorkSpace}\bin\debug\oiLogFile.log"
            del "${currentWorkSpace}\bin\debug\openIndyMath1.dll"
            del "${currentWorkSpace}\bin\debug\openIndyCore1.dll"
            RMDIR "${currentWorkSpace}\bin\debug\plugins"
			xcopy "${currentWorkSpace}\db\oisystemdb.sqlite" "${currentWorkSpace}\bin\debug"
			xcopy "${currentWorkSpace}\lib\OpenIndy-Core\bin\debug\openIndyCore1.dll" "${currentWorkSpace}\bin\debug"
			xcopy "${currentWorkSpace}\lib\OpenIndy-Core\lib\OpenIndy-Math\bin\debug\openIndyMath1.dll" "${currentWorkSpace}\bin\debug"/)
            bat(/xcopy \/e \/i \/y "C:\Bitnami\jenkins-2.19.4-0\apps\jenkins\jenkins_home\workspace\dependencies\OpenIndy\debug" "${currentWorkSpace}\bin\debug" /)
        }
        stage('compile release'){
            bat '''call "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat" x86_amd64
            "C:\\Qt\\5.5\\msvc2013_64\\bin\\qmake.exe" openIndy.pro -r -spec win32-msvc2013 "CONFIG+=release"
            nmake clean
            nmake'''
            bat "echo build openIndy release finished"
        }
        stage('prepare release folder'){
            bat (/del "${currentWorkSpace}\bin\release\oisystemdb.sqlite"
            del "${currentWorkSpace}\bin\release\oiLogFile.log"
            del "${currentWorkSpace}\bin\release\openIndyMath1.dll"
            del "${currentWorkSpace}\bin\release\openIndyCore1.dll"
            RMDIR "${currentWorkSpace}\bin\release\plugins"
			xcopy "${currentWorkSpace}\db\oisystemdb.sqlite" "${currentWorkSpace}\bin\release"
			xcopy "${currentWorkSpace}\lib\OpenIndy-Core\bin\release\openIndyCore1.dll" "${currentWorkSpace}\bin\release"
			xcopy "${currentWorkSpace}\lib\OpenIndy-Core\lib\OpenIndy-Math\bin\release\openIndyMath1.dll" "${currentWorkSpace}\bin\release"/)
            bat(/xcopy \/e \/i \/y "C:\Bitnami\jenkins-2.19.4-0\apps\jenkins\jenkins_home\workspace\dependencies\OpenIndy\release" "${currentWorkSpace}\bin\release" /)
        }
    } catch (e) {
        // If there was an exception thrown, the build failed
        currentBuild.result = "FAILED"
        throw e
    } finally {
        // Success or failure, always send notifications
        //notifyBuild(currentBuild.result)
    }
}