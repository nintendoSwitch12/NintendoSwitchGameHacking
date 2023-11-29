# Table of Contents
- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Usage](#usage)


# Overview

Nintendo Switch also have areas like GOT. So when you call the SDK function, it goes through the GOT. However, since there are no symbols in this area, it was difficult to analyze. So I wrote a script to follow the function and apply symbols to the GOT.

# Usage

When you create a `GOTReconstructor` object, you give it an address to start and an address to end the search, and then call the `Reconstruct` method.

```python
GOTReconstructor(0x0000000084536D70,0x000000008453A798).Reconstruct()
```