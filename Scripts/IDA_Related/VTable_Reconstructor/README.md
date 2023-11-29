# Table of Contents
- [Table of Contents](#table-of-contents)
- [Outline](#outline)
- [HexRaysCodeXplorer](#hexrayscodexplorer)
- [Modified to demangle method names.](#modified-to-demangle-method-names)
  - [Naming Routine](#naming-routine)
  - [Modification](#modification)
  - [Modified Source Code](#modified-source-code)
    - [with args](#with-args)
    - [without args](#without-args)
- [Post Processing](#post-processing)


# Outline

Versions below 3.1.0 in Splatoon 2 contain symbols of majority of class and functions in the binary. Therefore, we were able to create structures from the Virtual Tables used in classes for easier analysis of other games. This article talks about how to extract structs from Vtables in Splatoon 2.

# HexRaysCodeXplorer

One of the IDA plugins called HeyRaysCodeXplorer reconstruct VTables and classes in the data section[^1]. Using this plugin, in the case of a class with symbols, structures are created representing the class and the VTable respectively.

However, using the plugin in Splatoon 2 resulted in structures with mangled method names from VTable.

Therefore, we added a routine to demagle these method names. 

# Modified to demangle method names.

## Naming Routine

The routine for naming the method of VTable for each class exists in the buildReconstructibleTypeRecursive function in GCCOjectFormatParser.cpp. This function utilizes the IDA SDK to take the names of methods from the VTable and save them as a member of the structure.


## Modification

In order to demangled names of methods, a routine can be added in the buildReconstructableTypesRecursive function of GCCObjectFormatParser.cpp. Since the demangling function is already defined in IDA SDK, we can just use it[^2].

In case the demangled name is same as the mangled name, ‘__’ is added at the end of the name for differentiation. In addition, since certain special characters cannot be included in the member names of structures in IDA, they are replaced with other characters as below. 


- `,` → `_C_` (Comma)
- `` → `_S_` (Space)
- `~` → `_T_` (Tilde)
- `*` → `_P_` (Pointer)
- `=` → `_E_` (Equal)
- `<` → `_L_` (Little)
- `>` → `_G_` (Greater)

## Modified Source Code

### with args

[With args](./src/with_args/)

### without args

[Without args](./src/without_args/)

# Post Processing

If you try to load an extracted VTable from a different binary, it will cause an error since structure names cannot contain special characters such as `<`, ``, `,` and '*'. Therefore, we created a script that will change the structure name containing these characters. 
[Script](./src/post_process.py)

[^1]:https://github.com/zengfr/HexRaysCodeXplorer_plugin_for_ida_pro
[^2]: https://www.hex-rays.com/products/ida/support/idadoc/272.shtml
