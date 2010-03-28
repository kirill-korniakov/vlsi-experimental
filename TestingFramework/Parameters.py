class TestRunnerParameters:
    doCheckout = False
    doBuild    = False
    doSendMail = True

    experiments = []

class RepoParameters:
	repoPath = 'http://svn.software.unn.ru/VLSI/CODE/trunk'
	srcRepoPath = repoPath + '/itlPlace/'
	benchRepoPath = repoPath + '/Benchmarks/'

class GeneralParameters:
	checkoutPath = "../itlPlace/"
	binDir  = checkoutPath + "bin/"
	slnPath = checkoutPath + "make/itlPlace.sln"
	benchmarkCheckoutPath = "../Benchmarks/"

class EmailerParameters:
    #recipients = ['itlab.vlsi@www.software.unn.ru']
    #recipients = ['zhivoderov.a@gmail.com']
    recipients = ['kirill.kornyakov@gmail.com']

    smtpserver = 'mail.unn.ru'
    smtpuser   = ''
    smtppass   = ''
    sender     = 'VLSIMailerDaemon@gmail.com'

class Tools:
	MSBuild  = "c:/WINDOWS/Microsoft.NET/Framework/v2.0.50727/MSBuild.exe"
	UnRar = "7z.exe"