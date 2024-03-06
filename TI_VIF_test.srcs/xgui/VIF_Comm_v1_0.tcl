# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "RX_RecvTime" -parent ${Page_0}
  ipgui::add_param $IPINST -name "RX_Window" -parent ${Page_0}


}

proc update_PARAM_VALUE.RX_RecvTime { PARAM_VALUE.RX_RecvTime } {
	# Procedure called to update RX_RecvTime when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.RX_RecvTime { PARAM_VALUE.RX_RecvTime } {
	# Procedure called to validate RX_RecvTime
	return true
}

proc update_PARAM_VALUE.RX_Window { PARAM_VALUE.RX_Window } {
	# Procedure called to update RX_Window when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.RX_Window { PARAM_VALUE.RX_Window } {
	# Procedure called to validate RX_Window
	return true
}


proc update_MODELPARAM_VALUE.RX_Window { MODELPARAM_VALUE.RX_Window PARAM_VALUE.RX_Window } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.RX_Window}] ${MODELPARAM_VALUE.RX_Window}
}

proc update_MODELPARAM_VALUE.RX_RecvTime { MODELPARAM_VALUE.RX_RecvTime PARAM_VALUE.RX_RecvTime } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.RX_RecvTime}] ${MODELPARAM_VALUE.RX_RecvTime}
}

