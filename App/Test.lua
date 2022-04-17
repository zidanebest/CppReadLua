return {
	ID = "Sphere_200_360",
	LuaFilter = "CheckRangeHit",
	LuaFilterParaments = {
		Type = "Sphere",
		Radius = 200.0,
		RangeAngle = 360.0,
		[100] = {
		            RangeAngle={
                        LuaFilter = "CheckRangeHit" 
                    },
                    [200]=true
                 }
	},
	[100] = {
    	    RangeAngle=10,
    	    [200]=true
    	},
	CampFilter = "Enemy"

}



