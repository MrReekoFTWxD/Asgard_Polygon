#pragma once


template<typename T>
struct TArray
{
	friend struct FString;

	TArray(std::uint64_t addr = 0)
	{
		if (addr)
		{
			Data = driver.read<std::uint64_t>(addr);;
			Count = driver.read<std::uint32_t>(addr + 0x8);
			Max = driver.read<std::uint32_t>(addr + 0xC);
		}
	}

	[[nodiscard]] constexpr auto Num() const noexcept
	{
		return Count;
	};

	[[nodiscard]] constexpr T operator[](std::int32_t i) noexcept
	{
		return driver.read<T>(Data + (i * 0x8));
	};

	[[nodiscard]] constexpr const T operator[](std::int32_t i) const noexcept
	{
		driver.read<T>(Data + (i * 0x8));
	};

	[[nodiscard]] constexpr auto IsValidIndex(std::int32_t i) const noexcept
	{
		return i < Num();
	}

	T Cast(int index)
	{
		if (Count < index)
			return T();
		return driver.read<T>(Data + (index * 0x8));
	}

	std::uint64_t Data;
	std::int32_t Count;
	std::int32_t Max;
};

struct FString
{
	TArray<wchar_t> data;

	FString() {};

	FString(uint64_t offset)
	{
		data = offset;
	}

	std::string ToString() const
	{
		wchar_t* tempStr = new wchar_t[data.Count * 2];
		driver.readm((void*)data.Data, tempStr, data.Count * 2);
		std::string str(data.Count * 2, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(tempStr, tempStr + (data.Count * 2), '?', &str[0]);

		return str;
	}

	std::wstring ToWString()
	{
		wchar_t* tempStr = new wchar_t[data.Count * 2];
		driver.readm((void*)data.Data, tempStr, data.Count * 2);
		return tempStr;
	}
};

struct FNameEntryHandle
{
	uint32_t Block = 0;
	uint32_t Offset = 0;

	FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
	FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
	operator uint32_t() const { return (Block << 16 | Offset); }
};

struct FNameEntry
{
	uint16_t bIsWide : 1;
	uint16_t LowercaseProbeHash : 5;
	uint16_t Len : 10;
	union
	{
		char AnsiName[1024];
		wchar_t	WideName[1024];
	};

	std::string String()
	{
		if (bIsWide) { return std::string(); }
		return { AnsiName, Len };
	}
};

struct FNamePool
{
	byte Lock[8];
	uint32_t CurrentBlock;
	uint32_t CurrentByteCursor;
	byte* Blocks[8192];

	FNameEntry* GetEntry(FNameEntryHandle handle) const
	{
		return reinterpret_cast<FNameEntry*>(Blocks[handle.Block] + 2 * static_cast<uint64_t>(handle.Offset));
	};

	std::string GetName(int index) const
	{
		FNameEntry temp = FNameEntry();
		driver.readm((void*)this->GetEntry(index), &temp, sizeof(FNameEntry));
		return temp.String();
	}
};
extern FNamePool* NamePoolData;

struct FName
{
	uint32_t Index;
	uint32_t Number;

	FName() {}
	FName(uint64_t addr)
	{
		Index = driver.read<std::uint32_t>(addr);
		Number = driver.read<std::uint32_t>(addr + 0x4);
	}

	std::string GetName()
	{
		auto entry = NamePoolData->GetEntry(Index);
		auto name = entry->String();
		if (Number > 0)
		{
			name += '_' + std::to_string(Number);
		}
		auto pos = name.rfind('/');
		if (pos != std::string::npos)
		{
			name = name.substr(pos + 1);
		}
		return name;
	}
};

#pragma region Math


#define TORAD(x) ((x) * (M_PI / 180.f))
#define M_PI 3.14159265358979323846


inline float deg2rad(float deg)
{
	return deg * M_PI / 180.f;
}

inline float rad2deg(float radian)
{
	float pi = 3.14159;
	return(radian * (180 / pi));
}

struct FVector2D
{
	union
	{
		struct { double X, Y; };
		struct { double x, y; };
		struct { double pitch, yaw; };
		struct { double value[2]; };
	};

	FVector2D() : x(0), y(0) {}
	FVector2D(double x) : x(x), y(0) {}
	FVector2D(double x, double y) : x(x), y(y) {}
	FVector2D(double value[2]) : x(value[0]), y(value[1]) {}
};

struct FVector
{
	union
	{
		struct { double X, Y, Z; };
		struct { double x, y, z; };
		struct { double pitch, yaw, roll; };
		struct { double value[3]; };
	};

	constexpr FVector(double x = 0.f, double y = 0.f, double z = 0.f) noexcept : X{ x }, Y{ y }, Z{ z } {}

	[[nodiscard]] friend constexpr auto operator-(const FVector& a, const FVector& b) noexcept -> FVector
	{
		return { a.X - b.X, a.Y - b.Y, a.Z - b.Z };
	}

	[[nodiscard]] friend constexpr auto operator+(const FVector& a, const FVector& b) noexcept -> FVector
	{
		return { a.X + b.X, a.Y + b.Y, a.Z + b.Z };
	}

	[[nodiscard]] friend constexpr auto operator*(const FVector& a, const FVector& b) noexcept -> FVector
	{
		return { a.X * b.X, a.Y * b.Y, a.Z * b.Z };
	}

	[[nodiscard]] friend constexpr auto operator*(const FVector& v, double f) noexcept -> FVector
	{
		return { v.X * f, v.Y * f, v.Z * f };
	}

	[[nodiscard]] friend constexpr auto operator/(const FVector& v, double f) noexcept -> FVector
	{
		return { v.X / f, v.Y / f, v.Z / f };
	}

	[[nodiscard]] friend constexpr auto operator+(const FVector& v, double f) noexcept -> FVector
	{
		return { v.X + f, v.Y + f, v.Z + f };
	}

	[[nodiscard]] friend constexpr auto operator-(double f, const FVector& v) noexcept -> FVector
	{
		return{ f - v.X, f - v.Y, f - v.Z };
	}

	constexpr auto& operator+=(const FVector& v) noexcept
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	constexpr auto& operator-=(const FVector& v) noexcept
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}

	constexpr auto& operator*=(const FVector& v) noexcept
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;
		return *this;
	}

	constexpr auto& operator/=(const FVector& v) noexcept
	{
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;
		return *this;
	}

	const double DotProduct(FVector coords)
	{
		return (this->X * coords.X) + (this->Y * coords.Y) + (this->Z * coords.Z);
	}

	bool isValid()
	{
		return (this->X != 0 && this->Y != 0 && this->Z != 0);
	}

	double length()
	{
		return (double)sqrt(X * X + Y * Y + Z * Z);
	}

	std::string ToString()
	{
		return va("%.3f %.3f %.3f", this->x, this->y, this->z);
	}

	double distance(FVector vec)
	{
		double _x = this->X - vec.X;
		double _y = this->Y - vec.Y;
		double _z = this->Z - vec.Z;
		return sqrt((_x * _x) + (_y * _y) + (_z * _z));
	}

	FVector normalize()
	{
		FVector newvec;
		newvec.X = this->X / length();
		newvec.Y = this->Y / length();
		newvec.Z = this->Z / length();
		return newvec;
	}

	FVector GetSafeNormal(float tolerance = 1e-8f) const {
		const float squareSum = X * X + Y * Y + Z * Z;
		if (squareSum > tolerance) {
			const float scale = InvSqrt(squareSum);
			return FVector(X * scale, Y * scale, Z * scale);
		}
		return FVector(0,0,0);
	}

	FVector Rotation() const 
	{
		const float yaw = atan2(Y, X) * (180.0f / M_PI);
		const float pitch = atan2(Z, sqrt(X * X + Y * Y)) * (180.0f / M_PI);
		const float roll = 0.0f; // If you don't have roll data, you can set this to zero
		return FVector(yaw, pitch, roll);
	}

	FVector Lerp(const FVector& target, float alpha) const {
		return *this + (target - *this) * alpha;
	}

	static float InvSqrt(float x) {
		union {
			float f;
			int32_t i;
		} tmp;
		tmp.f = x;
		tmp.i = 0x5f3759df - (tmp.i >> 1);
		return 0.5f * tmp.f * (3.0f - x * tmp.f * tmp.f);
	}
};

struct FRotator
{
	union
	{
		struct { double X, Y, Z; };
		struct { double x, y, z; };
		struct { double pitch, yaw, roll; };
		struct { double Pitch, Yaw, Roll; };
		struct { double value[3]; };
	};

	[[nodiscard]] friend constexpr auto operator-(const FRotator& a, const FRotator& b) noexcept -> FRotator
	{
		return { a.Pitch - b.Pitch, a.Yaw - b.Yaw, a.Roll - b.Roll };
	}

	[[nodiscard]] friend constexpr auto operator+(const FRotator& a, const FRotator& b) noexcept -> FRotator
	{
		return { a.Pitch + b.Pitch, a.Yaw + b.Yaw, a.Roll + b.Roll };
	}

	[[nodiscard]] friend constexpr auto operator*(const FRotator& a, const FRotator& b) noexcept -> FRotator
	{
		return { a.Pitch * b.Pitch, a.Yaw * b.Yaw, a.Roll * b.Roll };
	}

	[[nodiscard]] friend constexpr auto operator*(const FRotator& a, double b) noexcept -> FRotator
	{
		return { a.Pitch * b, a.Yaw * b, a.Roll * b };
	}

	[[nodiscard]] friend constexpr auto operator/(const FRotator& a, double b) noexcept -> FRotator
	{
		return { a.Pitch / b, a.Yaw / b, a.Roll / b };
	}

	double* fromAngle() const
	{
		double temp[3] = {
			std::cos(deg2rad(Pitch)) * std::cos(deg2rad(Yaw)),
			std::cos(deg2rad(Pitch)) * std::sin(deg2rad(Yaw)),
			std::sin(deg2rad(Pitch))
		};

		return temp;
	}

	constexpr auto& operator+=(const FRotator& o) noexcept
	{
		Pitch += o.Pitch;
		Yaw += o.Yaw;
		Roll += o.Roll;
		return *this;
	}

	constexpr auto& normalize() noexcept
	{
		Pitch = std::isfinite(Pitch) ? std::remainder(Pitch, 360.f) : 0.f;
		Yaw = std::isfinite(Yaw) ? std::remainder(Yaw, 360.f) : 0.f;
		Roll = 0.f;
		return *this;
	}

};

struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};

struct FTransform
{
	struct FQuat Rotation;  // 0x0(0x20)
	struct FVector Translation;  // 0x20(0x18)
	char pad_56[8];  // 0x38(0x8)
	struct FVector Scale3D;  // 0x40(0x18)
	char pad_88[8];  // 0x58(0x8)

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = Translation.x;
		m._42 = Translation.y;
		m._43 = Translation.z;

		float x2 = Rotation.x + Rotation.x;
		float y2 = Rotation.y + Rotation.y;
		float z2 = Rotation.z + Rotation.z;

		float xx2 = Rotation.x * x2;
		float yy2 = Rotation.y * y2;
		float zz2 = Rotation.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * Scale3D.x;
		m._22 = (1.0f - (xx2 + zz2)) * Scale3D.y;
		m._33 = (1.0f - (xx2 + yy2)) * Scale3D.z;

		float yz2 = Rotation.y * z2;
		float wx2 = Rotation.w * x2;
		m._32 = (yz2 - wx2) * Scale3D.z;
		m._23 = (yz2 + wx2) * Scale3D.y;

		float xy2 = Rotation.x * y2;
		float wz2 = Rotation.w * z2;
		m._21 = (xy2 - wz2) * Scale3D.y;
		m._12 = (xy2 + wz2) * Scale3D.x;

		float xz2 = Rotation.x * z2;
		float wy2 = Rotation.w * y2;
		m._31 = (xz2 + wy2) * Scale3D.z;
		m._13 = (xz2 - wy2) * Scale3D.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

#pragma endregion

struct FMinimalViewInfo
{
	struct FVector Location;  // 0x0(0x18)
	struct FVector Rotation;  // 0x18(0x18)
	float FOV;  // 0x30(0x4)
	float DesiredFOV;  // 0x34(0x4)
};

namespace game
{
	extern uint64_t GWorld;
	extern uint64_t PersistentLevel;
	extern uint64_t ActorArray;
	extern uint64_t ActorCount;
	extern uint64_t OwningGameInstance;
	extern uint64_t LocalPlayer;
	extern uint64_t LocalPlayerState;
	extern uint64_t LocalPlayerPawn;
	extern uint64_t PlayerController;
	extern uint64_t PlayerCameraManager;
	extern FMinimalViewInfo CameraCachePrivate;

	extern uint8_t LocalPlayerTeam;
}

enum actor_flags
{
	NONE = 0,
	VISIBLE = 1 << 1,
	ENEMY = 1 << 2,
};

enum class EGunClass
{
	NONE,
	ASSAULT,
	SMG,
	MARKSMAN,
	LMG,
	SHOTGUN,
	SNIPER,
	PISTOL,
	REVOLVER,
	MAX,
};

struct AActor
{
	FVector Location;
	FVector Rotation;
	std::string Name;
	int health;
	int flags;

	uint8_t gun_type;
	uint8_t grenade_count;


	struct
	{
		//Body
		FVector headPos;
		FVector neckPos;
		FVector spine1Pos;
		FVector spine2Pos;
		FVector spine3Pos;
		FVector pelvisPos;

		//Arms
		FVector upperarmlPos;
		FVector lowerarmlPos;
		FVector handlPos;
		FVector upperarmrPos;
		FVector lowerarmrPos;
		FVector handrPos;

		//Legs
		FVector upperleglPos;
		FVector lowerleglPos;
		FVector footlPos;
		FVector upperlegrPos;
		FVector lowerlegrPos;
		FVector footrPos;

	}bone;
};
extern std::vector<AActor> Actors;
