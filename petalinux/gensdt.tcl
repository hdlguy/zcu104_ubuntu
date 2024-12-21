sdtgen set_dt_param -dir ./sdt
sdtgen set_dt_param -xsa ../implement/results/top.xsa                                    
sdtgen set_dt_param -include_dts ./system-user.dtsi
sdtgen generate_sdt      

