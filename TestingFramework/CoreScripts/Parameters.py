class TestRunnerParameters:
    svnRevision = ''

    doCheckout = False
    doBuild    = False

    cfg = "cfg/hpwl_ispd04.cfg" #path relative to bin directory
    useISPD04        = True
    doISPD04Checkout = True
    doISPD04BeforeDP = True
    doISPD04DP       = True
    benchmarks = '../../Benchmarks/ISPD04.list'

    doSendMail = True
    recipients = ['kirill.kornyakov@gmail.com']
    #RECIPIENTS = ['itlab.vlsi@www.software.unn.ru']
    #RECIPIENTS = ['zhivoderov.a@gmail.com']
    smtpserver = 'mail.unn.ru'
    smtpuser   = ''
    smtppass   = ''
    sender     = 'VLSIMailerDaemon@gmail.com'

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