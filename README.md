# LitColor
A lit color class to find color values within a program's memory dump.

## Constructors

### LitColor()
Default constructor. All colr alues are 0.
```
    LitColor val;
```

### LitColor(uint32_t rgba)
rgba = single integer containing values of RGBA.
  ```LitColor val(0x1E53CB);
     LitColor val(0x1E53CB91);
  ```
  
### LitColor(int32_t red, int32_t green, int32_t blue, int32_t alpha {optional})
Color values are being passed indifidually. Value range: 0 - 255.
```
   LitColor val(0x4E, 0x91, 0xFF);
   LitColor valRgba(0x4E, 0x91, 0xFF, 0x80);
```

### LitColor(float red, float green, float blue, float alpha {optional})
Color values are being passed indifidually. Value range: 0.0f - 1.0f.
```
   LitColor val(1.0, 0.5, 0.9);
   LitColor valRgba(1.0, 0.5, 0.9, 0.98); 
```

### template<typename T> LitColor<T>(T* rgbaPtr, bool containsAlpha {optional})
Color values originate from the givent pointer location of rgbaPtr. If containsAlpha is true, then the alpha value will be optained.
```
  int valsI[] = { 0x88, 0x66, 0xBB, 0xAA };
  float valsF[] = { 0.1, 0.2, 0.3};
  LitColor valRgba(valsI, true);
  LitColor val(valsF, false);
```
  
### LitColor(std::string expression)
Generate colors from std::string.
#### Formatting
- RGB Integer: "#RRGGBB"<br>
  ```
  LitColor val(std::string("#86E315");
  ```
- RGBA Integer: "#RRGGBBA"<br>
  ```
  LitColor val(std::string("#86E3159F");
  ```
- RGB Float: "R, G, B"<br>
  ```
  LitColor val(std::string("0.420, 0.69, 0.666");
  ```
- RGBA Float: "R, G, B, A"<br>
  ```
  LitColor val(std::string("0.420, 0.69, 0.666, 0.1337");
  ```
  
## Getters
### uint32_t GetRgba()
  Returns the rgba value.
  
### template<typename T> T GetColorValue(int colorIndicator)
  Returns a color value (red, green, blue or alpha) of type T.
  
### bool UsesAlpha()
  Expresses whether the alpha channel is considered.
  
## Setters
### void SetUseAlpha(bool shallI)
  Define whether to switch the alpha channel on or off.
  
## Operators
  ### = LitColor
  Set instance equal to the right-handed LitColor instance.
  
  ### = uint32_t
  Set instance equal to the rgba value.
  
  ### template<typename T> = T*
  Set instance equal to the array of values T* redirects to.
  
  ### bool == LitColor
  Checks for equality against the right-handed instance.
    
  ### bool == uint32_t
  Checks for equality against the rgba color value.
  
  ### template<typename T> bool == T*
  Checks for equality against the array T* redirects to.
  
  ### bool != LitColor
  Checks for inequality against the right-handed instance.
    
  ### bool != uint32_t
  Checks for inequality against the rgba color value.
  
  ### template<typename T> bool != T*
  Checks for inequality against the array T* redirects to.
  
  ### bool < LitColor
  Checks if the average color channel value is smaller than the right-handed instance's one.
    
  ### bool < uint32_t
  Checks if the average color channel value is smaller than the rgba value's one.
  
  ### template<typename T> bool < T*
  Checks if the average color channel value is smaller than values T* redirects to.
  
  ### bool <= LitColor
  Checks if the average color channel value is smaller than or equal to the right-handed instance's one.
    
  ### bool <= uint32_t
  Checks if the average color channel value is smaller than or equal to the rgba value's one.
  
  ### template<typename T> bool <= T*
  Checks if the average color channel value is smaller than or equal to values T* redirects to.
  
  ### bool > LitColor
  Checks if the average color channel value is smaller than the right-handed instance's one.
    
  ### bool > uint32_t
  Checks if the average color channel value is bigger than the rgba value's one.
  
  ### template<typename T> bool > T*
  Checks if the average color channel value is bigger than values T* redirects to.
  
  ### bool >= LitColor
  Checks if the average color channel value is bigger than or equal to the right-handed instance's one.
    
  ### bool >= uint32_t
  Checks if the average color channel value is bigger than or equal to the rgba value's one.
  
  ### template<typename T> bool >= T*
  Checks if the average color channel value is bigger than or equal to values T* redirects to.
  
  ### LitColor operator+(LitColor other)
  Returns the sum of this instance and the right-handed instance.
  
  ### LitColor operator+(uint32_t rgba)
  Returns the sum of this instance and the right-handed rgba value.
  
  ### template<typename T> LitColor operator+(T* valPtr)
  Returns the sum of this instance and the array of values T* redirects to.
  
  ### void operator+=(LitColor other)
  Adds the right-handed instance to itself.
  
  ### void operator+=(uint32_t rgba)
  Adds the rgba value to itself.
  
  ### template<typename T> void operator+=(T* valPtr)
  Adds the values T* redirects to to itself.
  
  ### LitColor operator-(LitColor other)
  Returns the difference of this instance and the right-handed instance.
  
  ### LitColor operator-(uint32_t rgba)
  Returns the difference of this instance and the right-handed rgba value.
  
  ### template<typename T> LitColor operator-(T* valPtr)
  Returns the difference of this instance and the array of values T* redirects to.
  
  ### void operator-=(LitColor other)
  Subtracts the right-handed instance from itself.
  
  ### void operator-=(uint32_t rgba)
  Subtracts the rgba value from itself.
  
  ### template<typename T> void operator-=(T* valPtr)
  Subtracts the values T* redirects to from itself.
  
  ### LitColor operator*(LitColor other)
  Returns the product of this instance and the right-handed instance.
  
  ### LitColor operator*(uint32_t rgba)
  Returns the product of this instance and the right-handed rgba value.
  
  ### template<typename T> LitColor operator*(T* valPtr)
  Returns the product of this instance and the array of values T* redirects to.
  
  ### void operator*=(LitColor other)
  Multiplies the right-handed instance with itself.
  
  ### void operator*=(uint32_t rgba)
  Multiplies the rgba value with itself.
  
  ### template<typename T> void operator*=(T* valPtr)
  Multiplies the values T* redirects to with itself.
  
  ### LitColor operator/(LitColor other)
  Returns the quotient of this instance with the right-handed instance.
  
  ### LitColor operator/(uint32_t rgba)
  Returns the quotient of this instance with the right-handed rgba value.
  
  ### template<typename T> LitColor operator/(T* valPtr)
  Returns the quotient of this instance with the array of values T* redirects to.
  
  ### void operator/=(LitColor other)
  Divides itself by the right-handed instnce.
  
  ### void operator/=(uint32_t rgba)
  Divides itself by the rgba value.
  
  ### template<typename T> void operator/=(T* valPtr)
  Divides itself by the values T* redirects to.
  
  ### LitColor operator&(LitColor other)
  Returns the ANDed result of this instance with the right-handed instance.
  
  ### LitColor operator&(uint32_t rgba)
  Returns the ANDed result of this instance with the rgba value.
  
  ### template<typename T> LitColor operator&(T* valPtr)
  Returns the ANDed result of this instance with the values T* redirects to.
  
  ### void operator&=(LitColor other)
  ANDs the right-handed instance with itself.
  
  ### void operator&=(uint32_t rgba)
  ANDs the rgba value with itself.
  
  ### template<typename T> void operator&=(T* valPtr)
  ANDs the values T* redirects to with itself.
  
  ### LitColor operator|(LitColor other)
  Returns the ORed result of this instance with the right-handed instance.
  
  ### LitColor operator|(uint32_t rgba)
  Returns the ORed result of this instance with the rgba value.
  
  ### template<typename T> LitColor operator|(T* valPtr)
  Returns the ORed result of this instance with the values T* redirects to.
  
  ### void operator|=(LitColor other)
  ORs the right-handed instance with itself.
  
  ### void operator|=(uint32_t rgba)
  ORs the rgba value with itself.
  
  ### template<typename T> void operator|=(T* valPtr)
  ORs the values T* redirects to with itself.
  
  ### LitColor operator^(LitColor other)
  Returns the XORed result of this instance with the right-handed instance.
  
  ### LitColor operator^(uint32_t rgba)
  Returns the XORed result of this instance with the rgba value.
  
  ### template<typename T> LitColor operator^(T* valPtr)
  Returns the XORed result of this instance with the values T* redirects to.
  
  ### void operator^=(LitColor other)
  XORs the right-handed instance with itself.
  
  ### void operator^=(uint32_t rgba)
  XORs the rgba value with itself.
  
  ### template<typename T> void operator^=(T* valPtr)
  XORs the values T* redirects to with itself.
  
  ### LitColor ~
  Returns the bitwise NOT of the color values.
  
  
  
