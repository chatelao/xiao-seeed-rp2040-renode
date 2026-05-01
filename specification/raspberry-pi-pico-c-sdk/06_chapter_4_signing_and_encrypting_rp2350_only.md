Raspberry Pi Pico-series C/C++ SDK

## **Chapter 4. Signing and encrypting (RP2350 only)**

You can securely sign, and optionally encrypt, binaries for the RP2350 chip. This forms the foundation of secure boot, which ensures only trusted code is run on the device.

Signing a binary allows you to validate the following using cryptographic signatures before running the binary:

- **[Origin.]**[ That the binary was created by a person or organisation in possession of a specific private key.]

- **[Integrity.]**[ That the binary hasn’t been modified since being signed.]

This validation is done by calculating a hash of the binary after compilation, signing this hash with a private key, and then attaching that signature and corresponding public key to the binary. The public key attached to the binary allows the RP2350’s boot ROM to verify that the signature is valid before running the binary. To ensure that only trusted public keys are accepted, the RP2350 uses OTP memory (BOOTKEY0–3) to store the hashes of authorised public keys.

In addition to signing a binary, you can also encrypt it to prevent anyone with access to the device from reading the data out of the encrypted binary in flash memory. This is done by encrypting the signed binary with a private key, and then storing that private key in the OTP memory. At boot, a bootloader can then read that private key and decrypt the binary before running it.

## **4.1. Signing binaries**

The RP2350 boot ROM supports secure boot, which allows you to restrict a device so that the boot ROM only runs code that has been signed by your private key.

An RP2350 with secure boot enabled doesn’t run an unsigned binary or a binary that has been signed with an incorrect private key. A signed binary still runs on an RP2350 that doesn’t have secure boot enabled, but the signature check doesn’t take place while loading. There are other steps you can take to further secure your device. For more information, see the Secure Boot Enable Procedure section in the **RP2350 Datasheet** .

To sign your binaries and enable secure boot on your device, start by creating a signing key for the secp256k1 curve in PEM format (for example, using openssl openssl ecparam -name secp256k1 -genkey -out private.pem) and then adding pico_sign_binary(<TARGET> /path/to/private.pem) to your CMakeLists.txt file. This generates an OTP JSON file named <TARGET>.otp.json for the target in the build directory. Load this OTP JSON file onto your device to enable secure boot using picotool:

picotool otp load <TARGET>.otp.json

You only need to load this OTP file once for each device. After the OTP is written, you can upload different signed binaries with the same signing key, without needing to rewrite the OTP.

4.1. Signing binaries

**79**

Raspberry Pi Pico-series C/C++ SDK

##  **WARNING**

This permanently modifies the OTP on your chip to enable secure boot. It’s therefore very important not to lose the private key that you’ve used for signing. If you lose the private key, you can’t sign any future binaries to run on your device, meaning that you can’t upgrade the code on your device.

Even if you sign your binary and enable secure boot, there are additional security considerations to take into account. The following provides a brief summary of the main points to consider when using secure boot. For more complete documentation, see the Bootrom chapter in the **RP2350 Datasheet** .

## **4.1.1. Flash versus SRAM**

When signing your binary, picotool adds a LOAD_MAP metadata item that specifies whether your code should be loaded into flash or SRAM. This gives you the option of having a packaged SRAM binary that is stored in flash but is loaded into SRAM for the signature check and execution.

If you need to protect against attackers with physical access to your device, you **can’t trust any code that is stored in flash** . This is because an attacker could remove your external flash chip, and then attach a multiplexer to switch between your flash chip and their own. This allows them to have your flash chip attached for the signature check so that the device trusts the code, and then switch over to their flash chip before boot to run their code instead. The chip variants with internal flash (RP2354A and RP2354B) don’t fully mitigate this issue because an attacker can still attach an external flash chip (or similar) with higher drive strength to the QSPI pins, allowing them to override the internal flash after the signature check.

Therefore, if you want to protect against attackers with physical access, you must ensure that your binary’s entry point is in SRAM, and never runs Secure code from flash. If your binary, along with any runtime memory it needs, fits inside 512 kB (plus 8 kB if you also use SRAM8-9, and 16 kB if you also use XIP_SRAM), then the easiest option is to use a Packaged SRAM binary. This can be done by adding the following to your CMakeLists.txt:

pico_set_binary_type(<TARGET> no_flash) pico_package_uf2_output(<TARGET> 0x10000000)

Setting the binary type to no_flash creates a binary that runs directly from SRAM. pico_package_uf2_output then changes the UF2 addresses to write the binary to the start of flash at 0x10000000 (or to the start of a partition if a partition table is present), rather than being loaded directly into SRAM.

If your binary is larger than the 512 kB of SRAM, you can designate some of the code as Non-Secure and run that from flash. If you do this, enable the SAU to prohibit Non-Secure access to Secure regions. For more information, see the Security Attribution and Memory Protection section in the **RP2350 Datasheet** . If the Non-Secure code also needs to access peripherals, set up Access Control accordingly. For more information, see the Access Control section in the **RP2350 Datasheet** . The SDK doesn’t provide support for creating this type of binary yet.

If you have a large binary where you can’t designate enough of the code as Non-Secure, use overlays and perform your own signature check when loading the overlays into SRAM; the SDK provides no support for this.

## **4.1.2. Example signed and packaged SRAM binary**

To create a signed and packaged SRAM binary, add the following lines to your CMakeLists.txt file, before the pico_add_extra_outputs(<TARGET>) line:

pico_set_binary_type(<TARGET> no_flash) pico_package_uf2_output(<TARGET> 0x10000000)

4.1. Signing binaries

**80**

Raspberry Pi Pico-series C/C++ SDK

## pico_sign_binary(<TARGET> /path/to/private.pem)

pico_set_binary_type and pico_package_uf2_output are explained in the Section 4.1.1 section. pico_sign_binary signs the binary using the private key.

To sign and package all binaries in your project (for example, in a project with multiple binaries for each device in the system), add the following lines just after pico_sdk_init():

set(PICO_DEFAULT_BINARY_TYPE no_flash) set_property(GLOBAL PROPERTY PICOTOOL_UF2_PACKAGE_ADDR 0x10000000) set_property(GLOBAL PROPERTY PICOTOOL_SIGFILE /path/to/private.pem) set_property(GLOBAL PROPERTY PICOTOOL_SIGN_OUTPUT TRUE)

This sets the default properties globally for all targets in the CMake project, whereas the CMake functions only set the properties for the specified target. Setting a property for a specific target overrides the property set globally.

## **4.2. Encrypting binaries**

In addition to signing your binary, if you have any secret code or data, you might want to store it encrypted in flash. This prevents anyone with access to the flash chip from reading your secrets. Encrypted code and data stored in flash must then be decrypted into SRAM on boot, which puts a limit of 512 kB on the size of the total encrypted code and data that can be used (plus the extra bits of SRAM mentioned in the Section 4.1.1 section). An encrypted binary requires the matching AES key and Initialisation Vector (IV) salt to be stored in the RP2350 OTP memory in order to run.

## **4.2.1. SDK encryption support**

The SDK provides support for creating symmetrically encrypted binaries, and also provides the option to embed a decryption stage into the binary to decrypt itself when run. You must pass it an AES key. We recommend using a 32-byte file of random data, which is expanded into a 4-way key share, described in the **Key Share and IV Salt** section.

There are two options for generating a self-decrypting binary:

- **[Hardened decryption (default).]**[ This option uses code specifically designed to resist side-channel attacks, making] it highly secure. This added protection can be slow, taking approximately 5 seconds to decrypt a 512 kB binary, enough to fully fill the RP2350’s SRAM. For more information about side-channel attacks, see the **Side-channel attacks** section.

- **[MbedTLS decryption.]**[ This option is faster, taking approximately 0.5 seconds to decrypt the same binary. However,] it includes fewer side-channel protections and is therefore less secure against an attacker with physical access. This might protect against a basic attacker with a flash reader, but a sophisticated attacker can analyse the power signature. For this reason, don’t rely on the MbedTLS implementation to protect against physical attacks.

## **4.2.2. Side-channel attacks**

Whenever you’re using symmetric keys for encryption and decryption (such as in AES), you risk side-channel attacks. Side-channel attacks allow an attacker to recover the encryption keys by analysing the timing, power consumption, and other physical signals during encryption or decryption. This isn’t a concern when encrypting the binary because that should be done on a secure computer without attacker access. However, decryption happens on the RP2350 device itself, which an attacker might have physical access to. To mitigate the risk, you can implement protections against side-channel attacks during the decryption stage.

For example, there could be a timing side-channel in your S-box lookup, such as described in this paper. There could also be a power side-channel when loading the key data because, at a very low level, it takes energy to flip bits but not to

4.2. Encrypting binaries

**81**

Raspberry Pi Pico-series C/C++ SDK

leave them set or clear. This means that when a register is overwritten, a power signature proportional to the number of bits changed between the old and new values is emitted.

## **4.2.3. Key Share and IV Salt**

For the hardened decryption stage, instead of storing the plain key in OTP, a 4-way key share is used. This key share means that the key X is stored as 4 separate values: A, B, C, and D, where X = A ^ B ^ C ^ D. This allows the decryption code to work on only one share of the key at a time (for example, 'A'), which reduces the risk of the key being leaked because even if you determine 'A' you can’t determine 'X' from 'A''. This provides good protection against power sidechannels.

The IV (Initialisation Vector) is also obscured by XORing it with the IV salt stored in OTP. AES is designed to be mathematically secure even to an attacker who knows the IV, but obscuring it from an attacker provides additional practical protection against side-channel attacks.

## **4.2.4. Example encrypted binary**

Before encrypting, you must generate an AES key and IV salt. If your computer has a cryptographically secure random number generator, the AES key can be generated using dd on Linux/MacOS:

dd if=/dev/urandom of=privateaes.bin bs=1 count=32

or using Powershell 7 on Windows:

[byte[]] $(Get-SecureRandom -Maximum 256 -Count 32) | Set-Content privateaes.bin -AsByteStream

You also need a per-device IV salt. This should be a 16-byte file of random data, which can be generated similarly:

dd if=/dev/urandom of=ivsalt.bin bs=1 count=16

##  **WARNING**

Keep the privateaes.bin and ivsalt.bin files for each device safe and secure. If you don’t keep these files secure, you can’t encrypt new data for that device without writing a new key and salt to OTP.

To create the default encrypted binary, which is a binary that fits into 512 kB of SRAM with the default decrypting bootloader running out of scratch SRAM8-9, you can add the following lines to your CMakeLists.txt file, before the pico_add_extra_outputs(<TARGET>) line.

pico_set_binary_type(<TARGET> no_flash) pico_package_uf2_output(<TARGET> 0x10000000) pico_sign_binary(<TARGET> /path/to/private.pem) pico_encrypt_binary(<TARGET> /path/to/privateaes.bin /path/to/ivsalt.bin EMBED)

The first three lines are explained in Section 4.1.2. pico_encrypt_binary then encrypts the binary using the private AES key and salts the IV with the per-device IV salt, and EMBED specifies that the default decryption stage should be embedded in the encrypted binary to create a self-decrypting binary.

4.2. Encrypting binaries

**82**

Raspberry Pi Pico-series C/C++ SDK

To do this for all binaries in your project, add the following lines just after pico_sdk_init()

set(PICO_DEFAULT_BINARY_TYPE no_flash) set_property(GLOBAL PROPERTY PICOTOOL_UF2_PACKAGE_ADDR 0x10000000) set_property(GLOBAL PROPERTY PICOTOOL_SIGFILE /path/to/private.pem) set_property(GLOBAL PROPERTY PICOTOOL_SIGN_OUTPUT TRUE) set_property(GLOBAL PROPERTY PICOTOOL_AESFILE /path/to/privateaes.bin) set_property(GLOBAL PROPERTY PICOTOOL_IVFILE /path/to/ivsalt.bin) set_property(GLOBAL PROPERTY PICOTOOL_EMBED_DECRYPTION TRUE)

Similarly to signing, these will output OTP JSON files for each target in the build directory named <TARGET>.otp.json, which can be used to write the AES key and IV salt to your device in addition to the secure boot setup.

4.2. Encrypting binaries

**83**
