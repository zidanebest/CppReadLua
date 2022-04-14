

local t={
    [1]=3,
    b=2,
    c={
      d=4
    },
    [2]={
      g={
        k="bsd"
      }  
    }
}

Sphere_200_360 = {
		ID = "Sphere_200_360",
		LuaFilter = "CheckRangeHit",
		LuaFilterParaments = {
			Type = "Sphere",
			Radius = 200.0,
			RangeAngle = 360.0
		},
		CampFilter = "Enemy"
	},
	
	
	[10] = {
		NodeId = 10,
		NodeType = "AttackCombo",
		SkillState = 0,
		ActiveBlendTime = 0.0,
		AnimPlayRate = 1.0
	},
	[1010111] = {
		NodeId = 1010111,
		NodeType = "AttackCombo",
		NextNodeId = 1010112,
		SkillState = 1010111,
		ActiveBlendTime = -1.0,
		AnimPath = "/Game/Asset/Char/Player/Char001_HeitaoNew/Montage/",
		AnimResource = "Girl_Blade_Attack01_01_Montage",
		AnimPlayRate = 1.3,
		IsEnableRootMotion = true,
		DisableBlendBone = true,
		SkillNodeEffects = {
			1010111
		}
	},

return t;

