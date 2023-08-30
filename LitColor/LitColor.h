#pragma once

#include <iostream>
#include <sstream>
#include <vector>

class LitColor
{
private:
	int32_t _redI = 0;
	int32_t _greenI = 0;
	int32_t _blueI = 0;
	int32_t _alphaI = 0;
	uint32_t _rgba = 0;
	uint16_t _rgb565 = 0;
	float _redF = 0;
	float _greenF = 0;
	float _blueF = 0;
	float _alphaF = 0;
	bool _useAlpha = true;
	int _typeSelect = 0;
	bool _hadValidSourceValue = true;

	void generateRgb565FromInt()
	{
		_rgb565 = ((_redI >> 3) << 11) | ((_greenI >> 2) << 5) | (_blueI >> 3);
	}

	void generateIntFromRgb565()
	{
		_redI = (_rgb565 >> 11) & 0x1F;
		_greenI = (_rgb565 >> 5) & 0x3F;
		_blueI = _rgb565 & 0x1F;

		_redI = (_redI << 3) | (_redI >> 2);
		_greenI = (_greenI << 2) | (_greenI >> 4);
		_blueI = (_blueI << 3) | (_blueI >> 2);
	}

	void generateFloatFromInt()
	{
		_redF = static_cast<float>(_redI) / 255.0f;
		_greenF = static_cast<float>(_greenI) / 255.0f;
		_blueF = static_cast<float>(_blueI) / 255.0f;
		_alphaF = static_cast<float>(_alphaI) / 255.0f;
	}

	void generateIntFromFloat()
	{
		_redI = static_cast<int>(_redF * 255.0f);
		_greenI = static_cast<int>(_greenF * 255.0f);
		_blueI = static_cast<int>(_blueF * 255.0f);
		_alphaI = static_cast<int>(_alphaF * 255.0f);
	}

	void generateRgbaFromInt()
	{
		_rgba = (_alphaI) | (_blueI << 8) | (_greenI << 16) | (_redI << 24);
	}

	void generateIntFromRgba()
	{
		_redI = _rgba >> 24;
		_greenI = (_rgba >> 16) & 0xFF;
		_blueI = (_rgba >> 8) & 0xFF;
		_alphaI = _rgba & 0xFF;
	}

	template<typename T> void validateAllColorMembers()
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			_hadValidSourceValue = _redF >= 0.0f && _redF <= 1.0f
				&& _greenF >= 0.0f && _greenF <= 1.0f
				&& _blueF >= 0.0f && _blueF <= 1.0f
				&& (_useAlpha ? (_alphaF >= 0.0f && _alphaF <= 1.0f) : true);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			_hadValidSourceValue = _redF >= 0 && _redF <= 0xFF
				&& _greenF >= 0 && _greenF <= 0xFF
				&& _blueF >= 0 && _blueF <= 0xFF
				&& (_useAlpha ? (_alphaF >= 0 && _alphaF <= 0xFF) : true);
		}
	}

	template<typename T> void validateColorValue(const T value)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			_hadValidSourceValue = value >= 0.0f && value <= 1.0f;
		}
		else if constexpr (std::is_integral_v<T>)
		{
			_hadValidSourceValue = value >= 0 && value <= 0xFF;
		}
	}

	template<typename T> void generateFromPtr(const T* rgba)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			_redF = *rgba;
			_greenF = *(rgba + 1);
			_blueF = *(rgba + 2);
			_alphaF = _useAlpha ? *(rgba + 3) : 1.0f;
			validateAllColorMembers<T>();
			generateIntFromFloat();
		}
		else if constexpr (std::is_integral_v<T>)
		{
			_redI = *rgba;
			_greenI = *(rgba + 1);
			_blueI = *(rgba + 2);
			_alphaI = _useAlpha ? *(rgba + 3) : 0xFF;
			validateAllColorMembers<T>();
			generateFloatFromInt();
		}

		generateRgbaFromInt();
	}

	template<typename T> T toMinMaxColorValue(T val)
	{
		if constexpr (std::is_floating_point_v<T>)
			if (val < 0.0f)
				return 0.0f;
			if (val > 1.0f)
				return 1.0f;

		if constexpr (std::is_integral_v<T>)
			if (val < 0)
				return 0;
			if (val > 0xFF)
				return 0xFF;

		return val;
	}

	static uint32_t addThreshold(const uint32_t a, const uint32_t b)
	{
		const uint32_t val = a + b;
		if (val > 255)
			return 255;

		return val;
	}

	static uint32_t subThreshold(const uint32_t a, const uint32_t b)
	{
		if (b > a)
			return 0;
		
		return a - b;
	}

	static uint32_t mulThreshold(const uint32_t a, const uint32_t b)
	{
		const uint32_t val = a * b;
		if (val > 255)
			return 255;

		return val;
	}

	bool isEqual(const uint32_t rgbaOther) const
	{
		if (_useAlpha)
			return _rgba == rgbaOther;
		return (_rgba & 0xFFFFFF00) == (rgbaOther & 0xFFFFFF00);
	}

public:
	LitColor(){}

	LitColor(const LitColor& other) : LitColor()
	{
		*this = other;
	}

	LitColor(const uint32_t rgba, const bool usesAlpha = true) : LitColor()
	{
		_rgba = rgba;
		generateRgb565FromInt();
		generateIntFromRgba();
		generateFloatFromInt();
		_useAlpha = usesAlpha;
		_typeSelect = usesAlpha ? RGBA8888 : RGB888;
	}

	LitColor(const uint16_t rgb565)
		: LitColor()
	{
		_rgb565 = rgb565;
		_useAlpha = false;
		_typeSelect = RGB565;
		generateIntFromRgb565();
		generateIntFromRgba();
		generateFloatFromInt();
	}

	LitColor(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 0xFF) : LitColor()
	{
		_redI = r;
		_greenI = g;
		_blueI = b;
		_alphaI = a;
		validateAllColorMembers<int32_t>();
		generateRgb565FromInt();
		generateRgbaFromInt();
		generateFloatFromInt();
	}

	LitColor(const float r, const float g, const float b, const float a = 1.0f) : LitColor()
	{
		_redF = r;
		_greenF = g;
		_blueF = b;
		_alphaF = a;
		validateAllColorMembers<float>();
		generateIntFromFloat();
		generateRgbaFromInt();
		generateRgb565FromInt();
	}

	template<typename T> LitColor(const T* rgba, const bool containsAlpha = true) : LitColor()
	{
		_useAlpha = containsAlpha;
		generateFromPtr<T>(rgba);
		generateIntFromFloat();
		generateRgbaFromInt();
		generateRgb565FromInt();
	}

	LitColor(std::string expression) : LitColor()
	{
		if (expression.empty())
			return;

		std::stringstream stream;

		if (expression[0] == '#')
		{
			_typeSelect = RGBA8888;
			expression.erase(0, 1);
			stream << expression;

			if (expression.size() == 6)
			{
				stream << "FF";
				_useAlpha = false;
				_typeSelect = RGB888;
			}
			else if (expression.size() < 5)
			{
				_useAlpha = false;
				_typeSelect = RGB565;
				stream >> std::hex >> _rgb565;
				generateIntFromRgb565();
				generateFloatFromInt();
				generateRgbaFromInt();
				return;
			}

			stream >> std::hex >> _rgba;
			generateIntFromRgba();
			generateFloatFromInt();
			generateRgb565FromInt();
			return;
		}

		std::string item;
		std::vector<float> itemList;
		stream << expression;

		while (std::getline(stream, item, ','))
			itemList.push_back(stof(item));

		_redF = itemList[0];
		_greenF = itemList[1];
		_blueF = itemList[2];

		if (itemList.size() > 3)
		{
			_alphaF = itemList[3];
			_typeSelect = RGBAF;
		}
		else
		{
			_alphaF = 1.0f;
			_useAlpha = false;
			_typeSelect = RGBF;
		}
		
		generateIntFromFloat();
		generateRgbaFromInt();
		generateRgb565FromInt();
	}

	enum Colors
	{
		RED = 0,
		GREEN = 1,
		BLUE = 2,
		ALPHA = 3
	};

	enum Types
	{
		RGB888 = 0,
		RGBA8888 = 1,
		RGBF = 2,
		RGBAF = 3,
		RGB565 = 4//,
		//RGB332 = 5,
		//RGB444 = 6,
		//RGB555 = 7,
		//RGB101010 = 8
	};

	uint32_t GetRGBA() const
	{
		return _rgba;
	}

	uint16_t GetRGB565() const
	{
		return _rgb565;
	}

	uint32_t GetRgbLeShift() const
	{
		return _rgba >> 8;
	}

	int GetSelectedType() const
	{
		return _typeSelect;
	}

	void SelectType(int type)
	{
		_typeSelect = type;
		_useAlpha = (type == RGBA8888 || type == RGBAF) ? true : false;
	}

	template<typename T> void SetColorValue(T value, const int colorIndicator)
	{
		validateColorValue<T>(value);
		value = toMinMaxColorValue(value);

		if constexpr (std::is_integral_v<T>)
		{
			switch (colorIndicator)
			{
			case RED:
				_redI = value;
				break;
			case GREEN:
				_greenI = value;
				break;
			case BLUE:
				_blueI = value;
				break;
			case ALPHA:
				_alphaI = value;
				break;
			}

			generateFloatFromInt();
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			switch (colorIndicator)
			{
			case RED:
				_redF = value;
				break;
			case GREEN:
				_greenF = value;
				break;
			case BLUE:
				_blueF = value;
				break;
			case ALPHA:
				_alphaF = value;
				break;
			}

			generateIntFromFloat();
		}

		generateRgbaFromInt();
		generateRgb565FromInt();
	}

	template<typename T> T GetColorValue(const int colorIndicator)
	{
		if constexpr (std::is_integral_v<T>)
		{
			switch (colorIndicator)
			{
			case RED: return _redI;
			case GREEN: return _greenI;
			case BLUE: return _blueI;
			case ALPHA: return _alphaI;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			switch (colorIndicator)
			{
			case RED: return _redF;
			case GREEN: return _greenF;
			case BLUE: return _blueF;
			case ALPHA: return _alphaF;
			}
		}

		return 0;
	}

	void SetUseAlpha(const bool shallI)
	{
		_useAlpha = shallI;
	}

	bool UsesAlpha() const
	{
		return _useAlpha;
	}

	void operator=(const LitColor& other)
	{
		_redI = other._redI;
		_greenI = other._greenI;
		_blueI = other._blueI;
		_alphaI = other._alphaI;
		_rgba = other._rgba;
		_rgb565 = other._rgb565;
		_redF = other._redF;
		_greenF = other._greenF;
		_blueF = other._blueF;
		_alphaF = other._alphaF;
		_useAlpha = other._useAlpha;
		_typeSelect = other._typeSelect;
		_hadValidSourceValue = other._hadValidSourceValue;
	}

	template<typename T> void operator=(const T* colors)
	{
		generateFromPtr(colors);
		generateRgbaFromInt();
		generateRgb565FromInt();
	}

	void operator=(const uint32_t rgba)
	{
		_rgba = rgba;
		generateIntFromRgba();
		generateFloatFromInt();
		generateRgb565FromInt();
	}

	void operator=(const uint16_t rgb565)
	{
		_rgb565 = rgb565;
		generateIntFromRgb565();
		generateRgbaFromInt();
		generateFloatFromInt();
	}

	bool operator==(const LitColor& other) const
	{
		return isEqual(other._rgba);
	}

	bool operator==(const uint32_t rgba) const
	{
		return isEqual(rgba);
	}

	template<typename T> bool operator==(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T>)
		{
			return _redI == *valPtr
				&& _greenI == *(valPtr + 1)
				&& _blueI == *(valPtr + 2)
				&& _useAlpha ? _alphaI == *(valPtr + 3) : true;
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return _redF == *valPtr
				&& _greenF == *(valPtr + 1)
				&& _blueF == *(valPtr + 2)
				&& _useAlpha ? _alphaF == *(valPtr + 3) : true;
		}

		return false;
	}

	bool operator!=(const LitColor& other) const
	{
		return !isEqual(other._rgba);
	}

	bool operator!=(const uint32_t rgba) const
	{
		return !isEqual(rgba);
	}

	template<typename T> bool operator!=(const T* valPtr) const
	{
		return !(*this == valPtr);
	}

	bool operator<(const LitColor& other) const
	{
		return _redF < other._redF && _greenF < other._greenF && _blueF < other._blueF && (_useAlpha ? _alphaF < other._alphaF : true);
	}

	bool operator<(const uint32_t rgba) const
	{
		return *this < LitColor(rgba);
	}

	template<typename T> bool operator<(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return *this < LitColor(valPtr);

		return false;
	}

	bool operator<=(const LitColor& other) const
	{
		return _redF <= other._redF && _greenF <= other._greenF && _blueF <= other._blueF && (_useAlpha ? _alphaF <= other._alphaF : true);
	}

	bool operator<=(const uint32_t rgba) const
	{
		return *this <= LitColor(rgba);
	}

	template<typename T> bool operator<=(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return *this <= LitColor(valPtr);

		return false;
	}

	bool operator>(const LitColor& other) const
	{
		return _redF > other._redF && _greenF > other._greenF && _blueF > other._blueF && (_useAlpha ? _alphaF > other._alphaF : true);
	}

	bool operator>(const uint32_t rgba) const
	{
		return !(*this <= LitColor(rgba));
	}

	template<typename T> bool operator>(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return !(*this <= LitColor(valPtr));

		return false;
	}

	bool operator>=(const LitColor& other) const
	{
		return _redF >= other._redF && _greenF >= other._greenF && _blueF >= other._blueF && (_useAlpha ? _alphaF >= other._alphaF : true);
	}

	bool operator>=(const uint32_t rgba) const
	{
		return !(*this < LitColor(rgba));
	}

	template<typename T> bool operator>=(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return !(*this < LitColor(valPtr));

		return false;
	}

	LitColor operator+(LitColor other)
	{
		other._redI = addThreshold(other._redI, _redI);
		other._greenI = addThreshold(other._greenI, _greenI);
		other._blueI = addThreshold(other._blueI, _blueI);
		other._alphaI = addThreshold(other._alphaI, _alphaI);
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		generateRgb565FromInt();
		return other;
	}

	LitColor operator+(const uint32_t rgba)
	{
		return *this + LitColor(rgba);
	}

	LitColor operator+(const float value)
	{
		LitColor altered = *this;
		altered._redF += value;
		altered._greenF += value;
		altered._blueF += value;

		if (_useAlpha)
			altered._redF += value;

		altered.generateIntFromFloat();
		altered.generateRgbaFromInt();
		altered.generateRgb565FromInt();
		return altered;
	}

	template<typename T> LitColor operator+(const T* valPtr)
	{
		return *this + LitColor(valPtr);
	}

	void operator+=(const LitColor other)
	{
		*this = *this + other;
	}

	void operator+=(const uint32_t rgba)
	{
		*this += LitColor(rgba);
	}

	void operator+=(const float value)
	{
		*this = *this + value;
	}

	template<typename T> void operator+=(const T* valPtr)
	{
		*this += LitColor(valPtr);
	}

	LitColor operator-(LitColor other) const
	{
		other._redI = subThreshold(_redI, other._redI);
		other._greenI = subThreshold(_greenI, other._greenI);
		other._blueI = subThreshold(_blueI, other._blueI);
		other._alphaI = subThreshold(_alphaI, other._alphaI);
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		other.generateRgb565FromInt();
		return other;
	}

	LitColor operator-(const uint32_t rgba) const
	{
		return *this - LitColor(rgba);
	}

	LitColor operator-(float value) const
	{
		LitColor altered = *this;
		altered._redF -= value;
		altered._greenF -= value;
		altered._blueF -= value;

		if (_useAlpha)
			altered._redF -= value;

		altered.generateIntFromFloat();
		altered.generateRgbaFromInt();
		altered.generateRgb565FromInt();
		return altered;
	}

	template<typename T> LitColor operator-(const T* valPtr)
	{
		return *this - LitColor(valPtr);
	}

	void operator-=(const LitColor other)
	{
		*this = *this - other;
	}

	void operator-=(const uint32_t rgba)
	{
		*this -= LitColor(rgba);
	}

	void operator-=(const float value)
	{
		*this = *this - value;
	}

	template<typename T> void operator-=(const T* valPtr)
	{
		*this -= LitColor(valPtr);
	}

	LitColor operator*(LitColor other) const
	{
		other._redI = mulThreshold(_redI, other._redI);
		other._greenI = mulThreshold(_greenI, other._greenI);
		other._blueI = mulThreshold(_blueI, other._blueI);
		other._alphaI = mulThreshold(_alphaI, other._alphaI);
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		other.generateRgb565FromInt();
		return other;
	}

	LitColor operator*(const uint32_t rgba) const
	{
		return *this * LitColor(rgba);
	}

	LitColor operator*(float factor) const
	{
		LitColor altered = *this;
		altered._redF *= factor;
		altered._greenF *= factor;
		altered._blueF *= factor;
		
		if(_useAlpha)
			altered._redF *= factor;

		altered.generateIntFromFloat();
		altered.generateRgbaFromInt();
		altered.generateRgb565FromInt();
		return altered;
	}

	template<typename T> LitColor operator*(const T* valPtr)
	{
		return *this * LitColor(valPtr);
	}

	void operator*=(const LitColor other)
	{
		*this = *this * other;
	}

	void operator*=(const uint32_t rgba)
	{
		*this *= LitColor(rgba);
	}

	template<typename T> void operator*=(const T* valPtr)
	{
		*this *= LitColor(valPtr);
	}

	LitColor operator/(LitColor other) const
	{
		other._redI = _redI / other._redI;
		other._greenI = _greenI / other._greenI;
		other._blueI = _blueI / other._blueI;
		other._alphaI = _alphaI / other._alphaI;
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		other.generateRgb565FromInt();
		return other;
	}

	LitColor operator/(const uint32_t rgba) const
	{
		return *this / LitColor(rgba);
	}

	LitColor operator/(float factor) const
	{
		LitColor altered(_rgba);
		altered._redF /= factor;
		altered._greenF /= factor;
		altered._blueF /= factor;

		if (_useAlpha)
			altered._redF /= factor;

		altered.generateIntFromFloat();
		altered.generateRgbaFromInt();
		altered.generateRgb565FromInt();
		return altered;
	}

	template<typename T> LitColor operator/(const T* valPtr)
	{
		return *this / LitColor(valPtr);
	}

	void operator/=(const LitColor other)
	{
		*this = *this / other;
	}

	void operator/=(const uint32_t rgba)
	{
		*this /= LitColor(rgba);
	}

	template<typename T> void operator/=(const T* valPtr)
	{
		*this /= LitColor(valPtr);
	}

	LitColor operator&(const LitColor other) const
	{
		return LitColor(other._rgba & _rgba);
	}

	LitColor operator&(const uint32_t rgba) const
	{
		return *this & LitColor(rgba);
	}

	template<typename T> LitColor operator&(const T* valPtr)
	{
		return *this & LitColor(valPtr);
	}

	void operator&=(const LitColor other)
	{
		*this = *this & other;
	}

	void operator&=(const uint32_t rgba)
	{
		*this &= LitColor(rgba);
	}

	template<typename T> void operator&=(const T* valPtr)
	{
		*this &= LitColor(valPtr);
	}

	LitColor operator|(const LitColor other) const
	{
		return LitColor(other._rgba | _rgba);
	}

	LitColor operator|(const uint32_t rgba) const
	{
		return *this | LitColor(rgba);
	}

	template<typename T> LitColor operator|(const T* valPtr)
	{
		return *this | LitColor(valPtr);
	}

	void operator|=(const LitColor other) const
	{
		*this | *this & other;
	}

	void operator|=(const uint32_t rgba) const
	{
		*this |= LitColor(rgba);
	}

	template<typename T> void operator|=(const T* valPtr)
	{
		*this |= LitColor(valPtr);
	}

	LitColor operator^(const LitColor other) const
	{
		return LitColor(other._rgba ^ _rgba);
	}

	LitColor operator^(const uint32_t rgba) const
	{
		return *this ^ LitColor(rgba);
	}

	template<typename T> LitColor operator^(const T* valPtr)
	{
		return *this ^ LitColor(valPtr);
	}

	void operator^=(const LitColor other) const
	{
		*this ^ *this & other;
	}

	void operator^=(const uint32_t rgba) const
	{
		*this ^= LitColor(rgba);
	}

	template<typename T> void operator^=(const T* valPtr)
	{
		*this ^= LitColor(valPtr);
	}

	LitColor operator~() const
	{
		return LitColor(~_rgba);
	}

	void Test() const
	{
		std::cout << "Uses Alpha: " << _useAlpha << std::endl;
		std::cout << "RGBA: #" << std::hex << _rgba << std::endl;
		std::cout << "RGB565: #" << std::hex << _rgb565 << std::endl;
		std::cout << "Red Int: " << std::hex << _redI << " - " << std::dec << _redI << std::endl;
		std::cout << "Green Int: " << std::hex << _greenI << " - " << std::dec << _greenI << std::endl;
		std::cout << "Blue Int: " << std::hex << _blueI << " - " << std::dec << _blueI << std::endl;
		std::cout << "Alpha Int: " << std::hex << _alphaI << " - " << std::dec << _alphaI << std::endl;
		std::cout << "Red Float: " << _redF << std::endl;
		std::cout << "Green Float: " << _greenF << std::endl;
		std::cout << "Blue Float: " << _blueF << std::endl;
		std::cout << "Alpha Float: " << _alphaF << std::endl;
	}

	bool HadValidColorSource() const
	{
		return _hadValidSourceValue;
	}

	static uint32_t RGB565ToRGB888(const uint16_t rgb565, uint8_t alpha = 0xFF)
	{
		uint32_t red = (rgb565 >> 11) & 0x1F;
		uint32_t green = (rgb565 >> 5) & 0x3F;
		uint32_t blue = rgb565 & 0x1F;

		red = (red << 3) | (red >> 2);
		green = (green << 2) | (green >> 4);
		blue = (blue << 3) | (blue >> 2);

		return  alpha | (blue << 8) | (green << 16) | (red << 24); ;
	}
};