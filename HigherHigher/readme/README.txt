First you need to create the Push Certificates and enable your application to get notified:

1. Log in to the iPhone Developer Connection Portal and click App IDs Ensure you have created an App ID without a wildcard. Wildcard IDs cannot use the push notification service. For example, our iPhone application ID looks something like AB123346CD.com.serverdensity.iphone

2. Click Configure next to your App ID and then click the button to generate a Push Notification certificate. A wizard will appear guiding you through the steps to generate a signing authority and then upload it to the portal, then download the newly generated certificate. This step is also covered in the Apple documentation.

3. Import your aps_developer_identity.cer into your Keychain by double clicking the .cer file. Launch Keychain Assistant from your local Mac and from the login keychain, filter by the Certificates category. You will see an expandable option called “Apple Development Push Services”

4. Expand this option then right click on “Apple Development Push Services” > Export “Apple Development Push Services ID123?. Save this as apns-dev-cert.p12 file somewhere you can access it.

5. Do the same again for the “Private Key” that was revealed when you expanded “Apple Development Push Services” ensuring you save it as apns-dev-key.p12 file.

6. These files now need to be converted to the PEM format by executing this command from the terminal:

openssl pkcs12 -clcerts -nokeys -out apns-dev-cert.pem -in apns-dev-cert.p12
openssl pkcs12 -nocerts -out apns-dev-key-enc.pem -in apns-dev-key.p12

Then to remove the passphrase execute:

openssl rsa -in apns-dev-key-enc.pem -out apns-dev-key.pem




$ curl http://www.openssl.org/source/openssl-1.0.0a.tar.gz | tar xz
$ cd openssl-1.0.0a
$ export CFLAGS="-arch i386"
$ export LDFLAGS="-arch i386"
$ ./config
$ make
$ lipo -info libssl.a
input file libssl.a is not a fat file
Non-fat file: libssl.a is architecture: i386
$ lipo -info libcrypto.a
input file libcrypto.a is not a fat file
Non-fat file: libcrypto.a is architecture: i386