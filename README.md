### Simple Windows Keylogger

This keylogger is not meant to be used for malicious purposes!

#### How it works
- It sets hooks to fetch the key events. As they normally are encoded we use windows decryption/to Ascii/UNicode to get from the number code to the designated character
- Special keys such as ALT, CTRL... will be saved in a special way.
- The log will be saved in a file in the same directory, which couled later be sent to a third party

## NOTE
This code is very simple and just intended to get a hold of windows apis/hooks. Due to its simple nature, any antivirus will likely detect it within seconds of its execution.
To test the code you'll have to mark it as an exception for the antivirus software!
