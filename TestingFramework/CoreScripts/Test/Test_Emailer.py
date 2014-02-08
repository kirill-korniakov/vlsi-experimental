from CoreScripts.Emailer import *


def test():
    print("Send test mail")

    subject = "Test Mail"
    text = "This is automatically generated mail. Please do not reply."
    attachmentFiles = []

    ep = EmailerParameters()
    ep.recipients = ['kirill.kornyakov@gmail.com']
    emailer = Emailer(ep)

    emailer.PrepareAndSendMail(subject, text, attachmentFiles)


test()