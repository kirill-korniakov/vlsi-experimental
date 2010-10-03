class TestRunnerParameters:
    doCheckout = False
    doBuild    = False
    doSendMail = False

    maxTimeForBenchmark = 3600 #one hour
    experiments = []

class EmailerParameters:
    recipients = ['itlab.vlsi@itlab.unn.ru']
    #recipients = ['zhivoderov.a@gmail.com']
    #recipients = ['kirill.kornyakov@gmail.com']
    #recipients = ['kirill.kornyakov@gmail.com', 'belyakov.alexander.a@gmail.com']
    #recipients = ['belyakov.alexander.a@gmail.com']
    #recipients = ''

    #sender     = 'VLSIMailerDaemon@gmail.com'
    #smtpserver = 'mail.unn.ru'
    #smtpuser   = ''
    #smtppass   = ''

    sender     = 'itlab.vlsi@yandex.ru'
    smtpserver = 'smtp.yandex.com'
    smtpuser   = 'itlab.vlsi@yandex.ru'  # for SMTP AUTH, set SMTP username here
    smtppass   = 'itlPlace'

    #sender = 'VLSIMailerDaemon@gmail.com'
    #smtpserver = 'smtp.gmail.com'
    #AUTHREQUIRED = 1                     # if you need to use SMTP AUTH set to 1
    #smtpuser = 'VLSIMailerDaemon@gmail.com'  # for SMTP AUTH, set SMTP username here
    #smtppass = '22JUL22:19:49'

class RepoParameters:
	repoPath = 'http://svn.software.unn.ru/VLSI/CODE/trunk'
	srcRepoPath = repoPath + '/itlPlace/'
	benchRepoPath = repoPath + '/Benchmarks/'

class ReportParameters:
    logFolder = 'Reports/'

class GeneralParameters:
	checkoutPath = "../itlPlace/"
	binDir  = checkoutPath + "bin/"
	slnPath = checkoutPath + "make/itlPlace.sln"
	benchmarkCheckoutPath = "../Benchmarks/"

class LogParserParameters:
    PFSTTableHeader   = 'Placement Flow Stages Table'
    PQATTableHeader   = 'Placement Quality Analysis Table'
    PFSTBorderPattern = '-----'

class Tools:
	MSBuild  = "c:/WINDOWS/Microsoft.NET/Framework/v2.0.50727/MSBuild.exe"
	UnRar = "7z.exe"
