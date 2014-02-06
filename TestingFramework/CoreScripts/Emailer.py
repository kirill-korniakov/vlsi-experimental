import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import COMMASPACE, formatdate
from base64 import b64encode
import os

from Logger import Logger
from ParametersParsing import EmailerParameters


class Emailer:
    logger = None

    def __init__(self, parameters=EmailerParameters()):
        self.logger = Logger()
        self.parameters = parameters

    def PrepareAndSendMail(self, subject, text, attachmentFiles):
        self.send_mail(
            self.parameters.sender,  # from
            self.parameters.recipients,  # to
            subject,  # subject
            text,  # text
            attachmentFiles,  # attachment files
            self.parameters.smtpserver,  # SMTP server
            25,  # port
            self.parameters.smtpuser,  # login
            self.parameters.smtppass,  # password
            0)  # TTLS

    def PrepareAndSendMailIfRequired(self, text, attachmentFiles):
        if self.parameters.doSendMail == True:
            subject = self.parameters.subject
            self.logger.Log("Sending email with results:\n%s" % text)
            self.PrepareAndSendMail(subject, text, attachmentFiles)

    def SendMessageAndExit(self, text, attachmentFiles=None):
        if not attachmentFiles: attachmentFiles = []
        self.PrepareAndSendMailIfRequired(text, attachmentFiles)
        exit(1)

    @staticmethod
    def send_mail(sender, to, subject, text, files=None, server="localhost", port=0, login="", passwd="", ttls=0,
                  debuglevel=0):  # debug level
        if not files: files = []
        assert type(to) == list
        assert type(files) == list

        msg = MIMEMultipart()
        msg["From"] = sender
        msg["To"] = COMMASPACE.join(to)
        msg["Date"] = formatdate(localtime=True)
        msg["Subject"] = subject

        msg.attach(MIMEText(text))
        #msg.attach(MIMEText(text, "html", "UTF-8"))

        for file in files:
            part = MIMEBase("application", "octet-stream")
            #part.set_payload(open(file, "rb").read())
            #encoders.encode_base64(part)
            file_content = open(file, "rb").read()
            part.set_payload(b64encode(file_content).decode("ascii"))
            part["Content-Transfer-Encoding"] = "base64"
            part.add_header("Content-Disposition", 'attachment; filename="%s"' % os.path.basename(file))
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