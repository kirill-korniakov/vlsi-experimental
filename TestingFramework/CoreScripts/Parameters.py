class TestRunnerParameters:
    doCheckout = False
    doBuild    = False

    useISPD04        = True
    doISPD04Checkout = True
    doISPD04BeforeDP = True
    doISPD04DP       = True

    useIWLS05        = True
    doIWLS05BeforeDP = True
    doIWLS05DP       = True

    doSendMail = True

    revision = ''

class RepoParameters:
	repoPath = 'http://svn.software.unn.ru/VLSI/CODE/trunk'
	srcRepoPath = repoPath + '/itlPlace/'
	benchRepoPath = repoPath + '/Benchmarks/'

class GeneralParameters:
	checkoutPath = "..\\..\\itlPlace\\"
	binDir   = checkoutPath + "bin\\"
	slnPath = checkoutPath + "make\\itlPlace.sln"
	benchmarkCheckoutPath = "..\\..\\Benchmarks\\"

	ispd04   = "ISPD04"
	iwls05   = "IWLS05"

class Tools:
	#MSBuild  = "c:\\WINDOWS\\Microsoft.NET\\Framework\\v3.5\\MSBuild.exe"
	UnRar = "7z.exe"