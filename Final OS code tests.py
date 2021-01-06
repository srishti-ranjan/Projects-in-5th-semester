#PART 1

from cryptography.fernet import Fernet
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText


mes=input(str("Enter the message you want to encrypt :: "))

key = Fernet.generate_key()
encoded_message = mes.encode()
f = Fernet(key)
encrypted_message = f.encrypt(encoded_message)

print("The encrypted message is :: ", encrypted_message)


mail_body = str(encrypted_message)
mail_content = mail_body

#The mail addresses and password
sender_address = 'srishtiranjan2910@gmail.com'
sender_pass = 'Hipmover2910'
receiver_address = 'srishtiranjan2910@gmail.com'

#Setup the MIME
message = MIMEMultipart()
message['From'] = sender_address
message['To'] = receiver_address
#The subject line
message['Subject'] = 'The sender has sent you an encrypted message'   

#The body and the attachments for the mail
message.attach(MIMEText(mail_content, 'plain'))

#Create SMTP session for sending the mail
session = smtplib.SMTP('smtp.gmail.com', 587) #use gmail with port
session.starttls() #enable security
session.login(sender_address, sender_pass) #login with mail_id and password
text = message.as_string()
session.sendmail(sender_address, receiver_address, text)
session.quit()

print('Mail Sent')


#PART 2
#Sending the key to the other person
#Setup the MIME
o = input("Do you want to send the receiver the key? ")
if(o == 'y' or o == 'Y'):
    message = MIMEMultipart()
    message['From'] = sender_address
    message['To'] = receiver_address
    #The subject line
    message['Subject'] = 'The sender has sent you the key'   
    key1 = str(key)
    #The body and the attachments for the mail
    message.attach(MIMEText(key1, 'plain'))

    #Create SMTP session for sending the mail
    session = smtplib.SMTP('smtp.gmail.com', 587) #use gmail with port
    session.starttls() #enable security
    session.login(sender_address, sender_pass) #login with mail_id and password
    text = message.as_string()
    session.sendmail(sender_address, receiver_address, text)
    session.quit()
    print('Mail Sent for the key')


#PART 3
op = input("Do you want to send the receiver the original text? If yes press 'y' if no press 'n' :: ")
if(op == 'y' or op == 'Y'):
    message = MIMEMultipart()
    message['From'] = sender_address
    message['To'] = receiver_address
    #The subject line
    message['Subject'] = 'The sender has sent you the key'
    
    d = f.decrypt(encrypted_message)
    dm = str(d)
    #The body and the attachments for the mail
    message.attach(MIMEText(dm, 'plain'))

    #Create SMTP session for sending the mail
    session = smtplib.SMTP('smtp.gmail.com', 587) #use gmail with port
    session.starttls() #enable security
    session.login(sender_address, sender_pass) #login with mail_id and password
    text = message.as_string()
    session.sendmail(sender_address, receiver_address, text)
    session.quit()
    print('Mail Sent with the original message')   
else:
    print("The original message is not sent")
