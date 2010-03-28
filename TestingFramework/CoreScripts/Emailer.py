import smtplib
import email
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import COMMASPACE, formatdate
from email import encoders
from base64 import b64encode
import os

import Parameters
from Parameters import *

class Emailer:
    def __init__(self, parameters = EmailerParameters()):
        self.parameters = parameters

    def SendResults(self, experiment, reportTable):
        attachmentFiles = list()

        attachmentFiles.append(experiment.cfg)
        attachmentFiles.append(experiment.benchmarks)
        attachmentFiles.append(reportTable)

        subject = experiment.name
        self.PrepareAndSendMail(subject, subject, attachmentFiles)

    def PrepareAndSendMail(self, subject, text, attachmentFiles):
        self.send_mail(
            self.parameters.sender,     # from
            self.parameters.recipients, # to
            subject,                    # subject
            text,                       # text
            attachmentFiles,            # attachment files
            self.parameters.smtpserver, # SMTP server
            25,                         # port
            self.parameters.smtpuser,   # login
            self.parameters.smtppass,   # password
            0)                          # TTLS
        print('Success!')

    def send_mail(self,
        sender,               # from e-mail address
        to,                   # to e-mail address list
        subject,              # message subject
        text,                 # text message
        files  = [],          # attachment files
        server = "localhost", # SMTP server
        port   = 0,           # SMTP server port
        login  = "",          # login if requared or ""
        passwd = "",          # password
        ttls   = 0,           # TTLS flag (0/1)
        debuglevel = 0):      # debug level
        assert type(to) == list
        assert type(files) == list

        msg = MIMEMultipart()
        msg['From'] = sender
        msg['To'] = COMMASPACE.join(to)
        msg['Date'] = formatdate(localtime = True)
        msg['Subject'] = subject

        msg.attach(MIMEText(text))
        #msg.attach(MIMEText(text, "html", "UTF-8"))

        for file in files:
            part = MIMEBase('application', "octet-stream")
            #part.set_payload(open(file, "rb").read())
            #encoders.encode_base64(part)
            file_content = open(file,"rb").read()
            part.set_payload(b64encode(file_content).decode('ascii'))
            part['Content-Transfer-Encoding'] = 'base64'
            part.add_header('Content-Disposition', 'attachment; filename="%s"' % os.path.basename(file))
            msg.attach(part)

        # connect to server and send the mail
        server = smtplib.SMTP(server, port)
        server.set_debuglevel(debuglevel)
        if ttls:
            server.ehlo()
            server.starttls()
            server.ehlo()
        if login != "":
            server.login(login, passwd)
        s = msg.as_string()
        server.sendmail(sender, to, s)
        server.quit()


def test():
    print("Send test mail")

    ep = EmailerParameters()
    ep.recipients = ['kirill.kornyakov@gmail.com']
    #ep.sender = 'from@gmail.com'
    #ep.smtpserver = 'smtp.gmail.com'
    #AUTHREQUIRED = 1                     # if you need to use SMTP AUTH set to 1
    #ep.smtpuser = 'VLSIMailerDaemon@gmail.com'  # for SMTP AUTH, set SMTP username here
    #ep.smtppass = '22JUL22:19:49'

    emailer = Emailer(ep)

    subject = "Test Mail"
    text = "This is automatically generated mail. Please do not reply."

    emailer.PrepareAndSendMail(
        subject,    # subject
        text,       # text
        [])         # attachment files

#test()