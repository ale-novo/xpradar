import xp

class PythonInterface:
    def XPluginStart(self):
        self.Name = "Radar"
        self.Sig = "radar.xppython3"
        self.Desc = "wxr radar"

        # analog
        self.tiltData = 1.0 # Radar Tilt
        self.gainData = 1.0 # Radar Gain
        self.intData = 1.0  # Radar screen Intensity

        # buttons
        self.stbyData = 0 # Radar STANDBY
        self.testData = 0 # Radar TEST
        self.wxcData = 0 # RADAR Weather Contour
        self.mapData = 0 # Radar MAP mode
        self.frzData = 0 # Radar FREEZE image

        # switches
        self.tgtData = 0 # Radar TARGET alert
        self.stabData = 0 # Radar Stabilization mode
        self.azmkData = 0 # radar Azimuth Marker

        self.tilt = xp.registerDataAccessor(
            "radar/wxr/tilt",
            xp.Type_Float,                                      # The types we support
            1,                                                  # Writable
            0, 0,                                               # No accessors for ints
            self.GetTilt, self.SetTilt,   # Accessors for floats
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.gain = xp.registerDataAccessor(
            "radar/wxr/gain",
            xp.Type_Float,                                      # The types we support
            1,                                                  # Writable
            0, 0,                                               # No accessors for ints
            self.GetGain, self.SetGain,   # Accessors for floats
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.int = xp.registerDataAccessor(
            "radar/wxr/int",
            xp.Type_Float,                                      # The types we support
            1,                                                  # Writable
            0, 0,                                               # No accessors for ints
            self.GetInt, self.SetInt,   # Accessors for floats
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.stby = xp.registerDataAccessor(
            "radar/wxr/stby",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetInt, self.SetInt,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.test = xp.registerDataAccessor(
            "radar/wxr/test",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetTest, self.SetTest,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.wxc = xp.registerDataAccessor(
            "radar/wxr/wxc",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetWxc, self.SetWxc,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.map = xp.registerDataAccessor(
            "radar/wxr/map",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetMap, self.SetMap,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.frz = xp.registerDataAccessor(
            "radar/wxr/frz",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetFrz, self.SetFrz,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.tgt = xp.registerDataAccessor(
            "radar/wxr/tgt",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetTgt, self.SetTgt,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.stab = xp.registerDataAccessor(
            "radar/wxr/stab",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetStab, self.SetStab,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        self.azmk = xp.registerDataAccessor(
            "radar/wxr/azmk",
            xp.Type_Int,                                      # The types we support
            1,                                                  # Writable
            self.GetAzmk, self.SetAzmk,   # Accessors for floats
            0, 0,
            0, 0,                                               # Accessors for doubles
            0, 0,                                               # No accessors for int arrays
            0, 0,                                               # No accessors for float arrays
            0, 0,                                               # No accessors for raw data
            0, 0)                                               # Refcons not used

        return self.Name, self.Sig, self.Desc

    def XPluginStop(self):
        if (self.tilt):
            xp.unregisterDataAccessor(self.tilt)

        if (self.gain):
            xp.unregisterDataAccessor(self.gain)

        if (self.int):
            xp.unregisterDataAccessor(self.int)

    def XPluginEnable(self):
        # Register datarefs with datareftool, to make debugging easier!
        for sig in ('com.leecbaker.datareftool', 'xplanesdk.examples.DataRefEditor'):
            dre = xp.findPluginBySignature(sig)
            if dre != xp.NO_PLUGIN_ID:
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/tilt')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/gain')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/int')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/stby')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/test')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/wxc')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/map')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/frz')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/tgt')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/stab')
                xp.sendMessageToPlugin(dre, 0x01000000, 'radar/wxr/azmk')

        return 1

    def XPluginDisable(self):
        pass

    def XPluginReceiveMessage(self, inFromWho, inMessage, inParam):
        pass

    def GetTilt(self, inRefcon):
        return self.tiltData
    def SetTilt(self, inRefcon, inValue):
        self.tiltData = inValue
        pass

    def GetGain(self, inRefcon):
        return self.gainData
    def SetGain(self, inRefcon, inValue):
        self.gainData = inValue
        pass

    def GetInt(self, inRefcon):
        return self.intData
    def SetInt(self, inRefcon, inValue):
        self.intData = inValue
        pass

    def GetStby(self, inRefcon):
        return self.stbyData
    def SetStby(self, inRefcon, inValue):
        self.stbyData = inValue
        pass

    def GetTest(self, inRefcon):
        return self.testData
    def SetTest(self, inRefcon, inValue):
        self.testData = inValue
        pass

    def GetWxc(self, inRefcon):
        return self.wxcData
    def SetWxc(self, inRefcon, inValue):
        self.wxcData = inValue
        pass

    def GetMap(self, inRefcon):
        return self.mapData
    def SetMap(self, inRefcon, inValue):
        self.mapData = inValue
        pass

    def GetFrz(self, inRefcon):
        return self.frzData
    def SetFrz(self, inRefcon, inValue):
        self.frzData = inValue
        pass

    def GetTgt(self, inRefcon):
        return self.tgtData
    def SetTgt(self, inRefcon, inValue):
        self.tgtData = inValue
        pass

    def GetStab(self, inRefcon):
        return self.stabData
    def SetStab(self, inRefcon, inValue):
        self.stabData = inValue
        pass

    def GetAzmk(self, inRefcon):
        return self.azmkData
    def SetAzmk(self, inRefcon, inValue):
        self.azmkData = inValue
        pass
