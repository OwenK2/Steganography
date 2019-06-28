# Steganography
Hide files/messages in images and decode them later using this c++ steganography cli

## How to Use
### Encoding
`steg -i <input filename> -o <output filename> [-s|-b] <data>`

| Parameter | Purpose |
|---------------|-----------------------------------------------------------------------------------|
| <data> | The file name to encode into the image or the string to encode if used with -s |
| -i \<filename\> | Specify the original image to encode data into |
| -o \<filename\> | Name of image to create with encoded data |
| -b \<0-8\> | Define bit depth (how many bits to encode with your data into each color channel) |
| -s \<string\> | Specify a string to encode instead of a file |

### Decoding
`steg [-f] <filename>`

| Parameter | Purpose |
|---------------|-----------------------------------------------------------------------------------|
| <filename> | The image to decode the data from |
| -f | Create a file from the decoded file/message even if the data is just text |



## How to Install
### On MAC/Linux
1. Run `make` in order to create program
2. Move steg to local bin with `mv ./steg /usr/local/bin`
3. Now as long as `/usr/local/bin` is in your path variable you can use steg
### On Windows (not tested)
1. Install ***GnuWin32*** or use ***VisualStudio*** to allow access to makefile usage
2. Run `make` in order to create program
3. Move to any folder specified in you path variable

## Credits
* Uses the stb_image and stb_image_write library files to read and write images (https://github.com/nothings/stb)
