---------------------------------------------------------------------------------
-- utl_Math.lua
-- Script-side implementation if common math functions
---------------------------------------------------------------------------------

LuaMathLib = {}

---------------------------------------------------------------------------------
-- NewVec3
---------------------------------------------------------------------------------
local function NewVec3()
	local newVec3 = {}
	newVec3.x = 0.0
	newVec3.y = 0.0
	newVec3.z = 0.0

	---------------------------------------------------------------------------------
	-- SetA
	---------------------------------------------------------------------------------
	local function SetA(v, x, y, z)
		v.x = x
		v.y = y
		v.z = z
	end

	---------------------------------------------------------------------------------
	-- SetB
	---------------------------------------------------------------------------------
	local function SetB(v1, v2)
		SetA(v1, v2.x, v2.y, v2.z)
	end

	---------------------------------------------------------------------------------
	-- Set
	---------------------------------------------------------------------------------
	function newVec3:Set( ... )
		local argCount = #arg
		if argCount == 3 then
			SetA( newVec3, arg[1], arg[2], arg[3] )
		elseif argCount == 1 then
			SetB( newVec3, arg[1] )
		end
	end

	---------------------------------------------------------------------------------
	-- SetX
	---------------------------------------------------------------------------------
	function newVec3:SetX( x )
		newVec3.x = x
	end

	---------------------------------------------------------------------------------
	-- SetY
	---------------------------------------------------------------------------------
	function newVec3:SetY( y )
		newVec3.y = y
	end

	---------------------------------------------------------------------------------
	-- SetZ
	---------------------------------------------------------------------------------
	function newVec3:SetZ( z )
		newVec3.z = z
	end

	---------------------------------------------------------------------------------
	-- GetX
	---------------------------------------------------------------------------------
	function newVec3:GetX()
		return newVec3.x
	end

	---------------------------------------------------------------------------------
	-- GetY
	---------------------------------------------------------------------------------
	function newVec3:GetY()
		return newVec3.y
	end

	---------------------------------------------------------------------------------
	-- GetZ
	---------------------------------------------------------------------------------
	function newVec3:GetZ()
		return newVec3.z
	end

	return newVec3
end

---------------------------------------------------------------------------------
-- NewVec3A
---------------------------------------------------------------------------------
local function NewVec3A()
	return NewVec3()
end

---------------------------------------------------------------------------------
-- NewVec3B
---------------------------------------------------------------------------------
local function NewVec3B( x, y, z )
	local newVec3 = nil

	-- TODO: Is this three string check as fast as we can make it?
	if type(x) == "number" and type(y) == "number" and type(z) == "number" then
		newVec3 = NewVec3()
		newVec3.x = x
		newVec3.y = y
		newVec3.z = z
	end

	return newVec3
end

---------------------------------------------------------------------------------
-- NewVec3C
---------------------------------------------------------------------------------
local function NewVec3C( v )
	local newVec3 = nil

	if v.x ~= nil and v.y ~= nil and v.z ~= nil then
		newVec3 = NewVec3B( v.x, v.y, v.z )
	end

	return newVec3
end

---------------------------------------------------------------------------------
-- NewEulerAngle
---------------------------------------------------------------------------------
local function NewEulerAngle()
	local newEulerAngle = {}
	newEulerAngle.roll = 0.0
	newEulerAngle.pitch = 0.0
	newEulerAngle.yaw = 0.0

	---------------------------------------------------------------------------------
	-- SetA
	---------------------------------------------------------------------------------
	local function SetA(e, roll, pitch, yaw)
		assert( e ~= nil )
		assert( type(roll) == "number" )
		assert( type(pitch) == "number" )
		assert( type(yaw) == "number" )
		e.roll = roll
		e.pitch = pitch
		e.yaw = yaw
	end

	---------------------------------------------------------------------------------
	-- SetB
	---------------------------------------------------------------------------------
	local function SetB(e1, e2)
		assert( e2 ~= nil )
		assert( e2.roll ~= nil )
		assert( e2.pitch ~= nil )
		assert( e2.yaw ~= nil )
		SetA(e1, e2.roll, e2.pitch, e2.yaw)
	end

	---------------------------------------------------------------------------------
	-- Set
	---------------------------------------------------------------------------------
	function newEulerAngle:Set( ... )
		local argCount = #arg
		if argCount == 3 then
			SetA( newEulerAngle, arg[1], arg[2], arg[3] )
		elseif argCount == 1 then
			SetB( newEulerAngle, arg[1] )
		end
	end

	---------------------------------------------------------------------------------
	-- SetRoll
	---------------------------------------------------------------------------------
	function newEulerAngle:SetRoll( roll )
		newEulerAngle.roll = roll
	end

	---------------------------------------------------------------------------------
	-- SetPitch
	---------------------------------------------------------------------------------
	function newEulerAngle:SetPitch( pitch )
		newEulerAngle.pitch = pitch
	end

	---------------------------------------------------------------------------------
	-- SetYaw
	---------------------------------------------------------------------------------
	function newEulerAngle:SetYaw( yaw )
		newEulerAngle.yaw = yaw
	end

	---------------------------------------------------------------------------------
	-- GetRoll
	---------------------------------------------------------------------------------
	function newEulerAngle:GetRoll()
		return newEulerAngle.roll
	end

	---------------------------------------------------------------------------------
	-- GetPitch
	---------------------------------------------------------------------------------
	function newEulerAngle:GetPitch()
		return newEulerAngle.pitch
	end

	---------------------------------------------------------------------------------
	-- GetYaw
	---------------------------------------------------------------------------------
	function newEulerAngle:GetYaw()
		return newEulerAngle.yaw
	end

	return newEulerAngle
end

---------------------------------------------------------------------------------
-- NewEulerAngleA
---------------------------------------------------------------------------------
local function NewEulerAngleA()
	return NewEulerAngle()
end

---------------------------------------------------------------------------------
-- NewEulerAngleB
---------------------------------------------------------------------------------
local function NewEulerAngleB( e )
	local newEulerAngle = nil
	if e.roll ~= nil and e.pitch ~= nil and e.yaw ~= nil then
		newEulerAngle = NewEulerAngle()
		newEulerAngle.roll = e.roll
		newEulerAngle.pitch = e.pitch
		newEulerAngle.yaw = e.yaw
	end

	return newEulerAngle
end

---------------------------------------------------------------------------------
-- NewEulerAngleC
---------------------------------------------------------------------------------
local function NewEulerAngleC( roll, pitch, yaw )
	local newEulerAngle = nil

	-- TODO: Is this three string check as fast as we can make it?
	if type(roll) == "number" and type(pitch) == "number" and type(yaw) == "number" then
		newEulerAngle = NewEulerAngle()
		newEulerAngle.roll = roll
		newEulerAngle.pitch = pitch
		newEulerAngle.yaw = yaw
	end

	return newEulerAngle
end

---------------------------------------------------------------------------------
-- Vec3
---------------------------------------------------------------------------------
function LuaMathLib:Vec3( ... )
	local argCount = #arg
	local newVec3 = nil
	if argCount == 0 then
		newVec3 = NewVec3A()
	elseif argCount == 3 then
		newVec3 = NewVec3B( arg[1], arg[2], arg[3] )
	elseif argCount == 1 then
		newVec3 = NewVec3C( arg[1] )
	end

	if newVec3 == nil then
		OutputDebug( LOGERROR, "Invalid arguments when creating vector.\n" )
	end
	assert( newVec3 ~= nil )

	return newVec3
end

---------------------------------------------------------------------------------
-- EulerAngle
---------------------------------------------------------------------------------
function LuaMathLib:EulerAngle( ... )
	local argCount = #arg
	local newEulerAngle = nil
	if argCount == 0 then
		newEulerAngle = NewEulerAngleA()
	elseif argCount == 1 then
		newEulerAngle = NewEulerAngleB( arg[1] )
	elseif argCount == 3 then
		newEulerAngle = NewEulerAngleC( arg[1], arg[2], arg[3] )
	end

	if newEulerAngle == nil then
		OutputDebug( LOGERROR, "Invalid arguments when creating euler angle.\n" )
	end
	assert( newEulerAngle ~= nil )

	return newEulerAngle
end

---------------------------------------------------------------------------------
-- EulerToVector
---------------------------------------------------------------------------------
function LuaMathLib:EulerToVector( e, v )
	if v == nil then
		v = self:Vec3()
	end
	v.y = math.sin( math.rad( e.pitch ) )
	local yawLength = math.cos( math.rad( e.pitch ) )
	v.z = math.cos( math.rad( e.yaw ) ) * yawLength
	v.x = math.sin( math.rad( e.yaw ) ) * yawLength

	return v
end

---------------------------------------------------------------------------------
-- VectorToEuler
---------------------------------------------------------------------------------
function LuaMathLib:VectorToEuler( v, e )
	if e == nil then
		e = self:EulerAngle()
	end
	local vec = self:Vec3( v )
	LuaMathLib:Vec3Normalize( vec )
	e.pitch = math.deg( math.asin(vec.y) )
	e.roll = 0
	e.yaw = math.deg( math.atan2( -vec.z, vec.x ) ) + 90

	return e
end

---------------------------------------------------------------------------------
-- Vec3AddEquals
---------------------------------------------------------------------------------
function LuaMathLib:Vec3AddEquals( v1, v2 )
	v1.x = v1.x + v2.x
	v1.y = v1.y + v2.y
	v1.z = v1.z + v2.z
end

---------------------------------------------------------------------------------
-- Vec3SubtractEquals
---------------------------------------------------------------------------------
function LuaMathLib:Vec3SubtractEquals( v1, v2 )
	v1.x = v1.x - v2.x
	v1.y = v1.y - v2.y
	v1.z = v1.z - v2.z
end

---------------------------------------------------------------------------------
-- Vec3TimesEquals
---------------------------------------------------------------------------------
function LuaMathLib:Vec3TimesEquals( v, a )
	v.x = v.x * a
	v.y = v.y * a
	v.z = v.z * a
end

---------------------------------------------------------------------------------
-- Vec3DivideEquals
---------------------------------------------------------------------------------
function LuaMathLib:Vec3DivideEquals( v, a )
	v.x = v.x / a
	v.y = v.y / a
	v.z = v.z / a
end

---------------------------------------------------------------------------------
-- Vec3LengthSQ
---------------------------------------------------------------------------------
function LuaMathLib:Vec3LengthSQ( v )
	return v.x*v.x + v.y*v.y + v.z*v.z
end

---------------------------------------------------------------------------------
-- Vec3Length
---------------------------------------------------------------------------------
function LuaMathLib:Vec3Length( v )
	return math.sqrt( LuaMathLib:Vec3LengthSQ( v ) )
end

---------------------------------------------------------------------------------
-- Vec3SetLength
---------------------------------------------------------------------------------
function LuaMathLib:Vec3SetLength( v, newLength )
	LuaMathLib:Vec3Normalize( v )
	LuaMathLib:Vec3TimesEquals( v, newLength )
end

---------------------------------------------------------------------------------
-- Vec3Dot
---------------------------------------------------------------------------------
function LuaMathLib:Vec3Dot( v1, v2 )
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z))
end

---------------------------------------------------------------------------------
-- Vec3Cross
---------------------------------------------------------------------------------
function LuaMathLib:Vec3Cross( v1, v2 )
	return LuaMathLib:Vec3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x))
end

---------------------------------------------------------------------------------
-- Vec3DistanceBetweenSQ
---------------------------------------------------------------------------------
function LuaMathLib:Vec3DistanceBetweenSQ( v1, v2 )
	local vx = v1.x - v2.x
	local vy = v1.y - v2.y
	local vz = v1.z - v2.z
	return vx*vx + vy*vy + vz*vz
end

---------------------------------------------------------------------------------
-- Vec3DistanceBetween
---------------------------------------------------------------------------------
function LuaMathLib:Vec3DistanceBetween( v1, v2 )
	return math.sqrt( LuaMathLib:Vec3DistanceBetweenSQ( v1, v2 ) )
end

---------------------------------------------------------------------------------
-- Vec3Invert
---------------------------------------------------------------------------------
function LuaMathLib:Vec3Invert( v )
	v.x = -v.x
	v.y = -v.y
	v.z = -v.z
end

---------------------------------------------------------------------------------
-- Vec3Normalize
---------------------------------------------------------------------------------
function LuaMathLib:Vec3Normalize( v )
	local length = LuaMathLib:Vec3Length( v )
	if length ~= 0.0  then
		length = 1/length
		v.x = v.x * length
		v.y = v.y * length
		v.z = v.z * length
	end
end

---------------------------------------------------------------------------------
-- Vec3RotateAboutX
---------------------------------------------------------------------------------
function LuaMathLib:Vec3RotateAboutX( v, degRotation )
	local radAngle = degRotation * (math.pi / 180.0)
	local sinTheta = math.sin(radAngle)
	local cosTheta = math.cos(radAngle)

	v.y = (v.z * sinTheta) + (v.y * cosTheta)
	v.z = (v.z * cosTheta) - (v.y * sinTheta)
end
	
---------------------------------------------------------------------------------
-- Vec3RotateAboutY
---------------------------------------------------------------------------------
function LuaMathLib:Vec3RotateAboutY( v, degRotation )
	local radAngle = degRotation * (math.pi / 180.0)
	local sinTheta = math.sin(radAngle)
	local cosTheta = math.cos(radAngle)

	v.x = (v.z * sinTheta) + (v.x * cosTheta)
	v.z = (v.z * cosTheta) - (v.x * sinTheta)
end

---------------------------------------------------------------------------------
-- Vec3RotateAboutZ
---------------------------------------------------------------------------------
function LuaMathLib:Vec3RotateAboutZ( v, degRotation )
	local radAngle = degRotation * (math.pi / 180.0)
	local sinTheta = math.sin(radAngle)
	local cosTheta = math.cos(radAngle)

	v.x = (v.y * sinTheta) + (v.x * cosTheta )
	v.y = (v.y * cosTheta) - (v.x * sinTheta )
end