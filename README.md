# unname

This is small utility DLL to override UnDecorateSymbolName  function in some of Total Commander plugins.

To use, open them in Hex (or more handy) editor and change references from Dbghelp.dll to unname32.dll or unname64.dll. Be aware that this library has only UnDecorateSymbolName and if plugin uses other from dbghelp, it will crash.

 
