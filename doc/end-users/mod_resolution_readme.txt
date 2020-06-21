Modification to allow the user to specify any video resolution for the game "Dead or Alive 5: Last Round".
Game version(s): 1.10c (Steam)
================================================================================


Disclaimer
--------------------------------------------------------------------------------

Please be aware that developers of third party modifications cannot be held liable for any damage caused from using them.
It should go without saying that using third party modifications may not be safe when used with VAC-enabled games.


Installation
--------------------------------------------------------------------------------

Place the following files in your DOA5LR install directory:
    d3d9.dll
    mod_resolution.ini

Open the config file "mod_resolution.ini" with a text editor (e.g. Notepad) and specify a custom video resolution (e.g. 2560x1440).

If you need to use additional graphical mods (e.g. SweetFX) that provide their own Direct3D9 proxy libraries (d3d9.dll), they may co-exist:
    1. Rename any other files named "d3d9.dll". For SweetFX, I suggest "d3d9_sweetfx.dll".
    2. In the config section "ProxyChain_D3D9", add an entry for the file you just renamed. Example:

       [ProxyChain_D3D9]
       SweetFX=d3d9_sweetfx.dll

       You may disable a library by prepending ";" like this:

       [ProxyChain_D3D9]
       ;SweetFX=d3d9_sweetfx.dll

       Libraries will be loaded in the order they were specified in this section.
       NOTE: Currently, only a single proxy library is supported.


Developer
--------------------------------------------------------------------------------

Steffen André langnes
https://www.steffenl.com
