import smtplib
import email
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import COMMASPACE, formatdate
from email import encoders
from base64 import b64encode
import os

def send_mail(
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


if __name__ == "__main__":
    print("Send mail")
    smtpserver = 'smtp.gmail.com'
    #AUTHREQUIRED = 1                     # if you need to use SMTP AUTH set to 1
    smtpuser = 'VLSIMailerDaemon@gmail.com'  # for SMTP AUTH, set SMTP username here
    smtppass = '22JUL22:19:49'
    msg = "Some message to send"
    subject = "Test mail"

    RECIPIENTS = ['zhivoderov.a@gmail.com']
    SENDER = 'from@gmail.com'

    text = "This is automatically generated mail. Please do not reply."

    send_mail(
        SENDER,     # from
        RECIPIENTS, # to
        subject,    # subject
        text,       # text
        [],         # attachment files
        smtpserver, # SMTP server
        587,        # port
        smtpuser,   # login
        smtppass,   # password
        1)          # TTLS        
