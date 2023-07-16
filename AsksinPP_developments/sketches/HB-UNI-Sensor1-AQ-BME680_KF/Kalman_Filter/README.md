# A [Kalman filter](https://en.wikipedia.org/wiki/Kalman_filter) is used for estimating the regression coefficents for compensating the dependency of the BME680's gas resistance on the temperature and on the absolute humidity.

- Instead of an external offline '[Multiple Linear Regression](https://de.wikipedia.org/wiki/Lineare_Regression#Multiple_lineare_Regression)' (see also [wikiversity](https://en.m.wikiversity.org/wiki/Multiple_linear_regression)) used for the sensor [HB-UNI-Sensor1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680), this sensor [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF) is using a Kalman filter for an online calculation the regression coefficients.
- A Kalman filter with a zero covariance matrix for the process noise is well known as the recursive minimum least-square error (LMMSE) filter for a linear system with some assumptions on auto- and cross-correlations of process and measurement noise and initial state.
- See also '[Optimum Linear Estimation](https://www.sciencedirect.com/topics/social-sciences/kalman-filter)'
- The below Python Jupyterlab Kalman filter 'kalman-filter-AQ.ipynb' is derived from Kalman Filter is derived from [Zaur Fataliyev's](https://github.com/zziz) Github [ zziz /
kalman-filter](https://github.com/zziz/kalman-filter) repository. Thanks to Zaur for this great repository. 
- The corresponding C++ implementation of the Kalman filter in '[sens_bme680_KF.h](../sensors/sens_bme680_KF.h)' is based on [Romain JL. FETICK's](https://github.com/rfetick) Github  [rfetick/Kalman](https://github.com/rfetick/Kalman) repository 'Implement Kalman filter for your Arduino projects'. Special thanks to Romain for this great repository.
- For executing an online linear regression, it is essential to set the system model's covariance matrix Q to a zero matrix. Furthermore, the measurement covariance matrix R should bet set to a very small value, e.g. 0.0001 or smaller.

## Required Python installations on your computer

- Please ensure that you have installed the following Python modules on your computer (e.g. by pip3 install / .deb package for a Debian Linux distribution (preferred):
	+ pandas  				 / python3-pandas
	+ numpy    				 / python3-numpy
	+ matplotlib              / python3-matplotlib
	+ sklearn                 / python3-sklearn
	+ statsmodels             / python3-statsmodels
	+ datetime                / python3-dateutil
	+ jupyter-notebook        / jypyter-notebook
- This recipe was tested with Kubuntu 22.04 without any installation of a python module by pip3

## For getting familiar with the implemented Kalman filter please execute the following commands

- Invoke in a terminal window from the command line 'jupyter-notebook'
- Your default browser should now pop-up with the following URL:
> http://localhost:8888/tree
  
- Here a screen dump of the initial 'jupyter-notebook' browser window:
  ![pic](./jupyter-notebook.png)
 - Then run in 'jupyter-notebook' by clicking to the concerning scripts:
	+ kalman-filter-AQ.ipynb
	+ Multiple linear regression for BME680 gas readings of a single sensor.ipynb

- Compare the linear regression results at the end of the notebook
- The calculations can take several minutes depending on the size of historian.csv


## Steps for creating your own '[historian.csv](./historian.csv)'

- Before starting to collect data for a Kalman filter regression, it is necessary to calibrate the temperature and humidity measurements of the BME680 sensor. For that purpose, please track the BME680 sensor's temperature and humidity e.g. in the CCU Historian and compare them with 'golden' reference sensor's values. Extract the offsets between the reference temperature/humidity and the BM680 temperature/humidity measurements, ideally during night when there are little disturbances and thus flat curves, and use them for setting the temperature and humidity offsets in the WebUI.** Please run the offset calibration in two separated sequential steps**:
	+ calibrate the BME680 temperature offset in a first step (first night)
	+ calibrate then the BME680 humidity offset in a second step (second night)
	- ** do not calibrate temperature and humidity together in a singular step since they depend on each other from physics point of view**

- Collect history data with your HB-UNI-Sensor1-AQ-BME680_KF and record them with CCU Historian.
	Take care to have as many as possible different air quality, temperature, and humidity conditions during the recording time:
	- put the sensor into your kitchen when you are cooking
	- put the sensor to other locations with strong smells
	- place strong smelling food e.g. like Parmesan cheese close to the sensor<br/>
- Collect history data for 2..4 weeks
- Output a history CSV file in the CCU Historian's Trend diagram (button 'CSV-Export' at the bottom) dumping the following data points (click in CCU Historian's 'Datenpunktliste' to create the Trend diagram):

	- HB-UNI-Sensor1-AQ-BME680_KF's AQ_GAS_RESISTANCE_RAW
	- HB-UNI-Sensor1-AQ-BME680_KF's TEMPERATURE
	- HB-UNI-Sensor1-AQ-BME680_KF's MY_HUMIDITY <br/>

- Ensure that your browser is downloading the CSV file to your ${HOME}/Downloads directory as 'historian.csv'. Check the correct time stamp of that file before continuing. <br/>

- Execute the provided script '[get_new_history.bsh](./get_new_history.bsh)'. It fetches CCU Historian's CSV file from '${HOME}/Downloads' and does some pre-processing<br/>

- If you read in the created CSV file into Microsoft Office EXCEL or LibreOffice Calc, it should look like:

 ![ ](./EXCEL_Calc_view.png  "converted CSV view in EXCEL/Calc")

 - For the CSV import into EXCEL or LibreOffice Calc, please select a column separator the semicolon ';'
 Please check that the decimal separator is a **','**, the thousands separator should be a **'.'**. If you use a non German language setting on your computer, e.g. Austrian, you may need to adapt the thousands separator in the provided Jupyter notebooks 'kalman-filter-AQ.ipynb and 'Multiple linear regression for BME680 gas readings of a single sensor.ipynb' appropriately:
 
>  df0 = pd.read_csv("historian.csv", sep=';', thousands=".", decimal=",", skiprows = [0,1,2],dtype={'High': np.float64, 'Low': np.float64}, header = None, encoding= 'unicode_escape',  parse_dates=[0], date_parser=dateparse, names = [ 'Datum', 'Mode', 'raw_gas_resistance', 'relative_humidity', 'temperature'])


# Prove of the correctness of an online regression by the Kalman filter by using synthesized sensor data

- A Jupyter notebook ["Prove_of_Kalman_filter_with_synthesized_data.ipynb"](./Prove_of_Kalman_filter_with_synthesized_data.ipynb) is provided.
- It can be used for the following purposes for a better understanding and prove of the correct online regression by the Kalman filter by using synthesized sensor data:
+	Creation of synthesized sensor data (gas_resistance_raw, temperature, relative humidity)
+	gas_resistance_raw is synthesized as a linear equation of gas_resistance_compensated, temperature, and relative_humidity
+	The synthesis parameters 'alpha' and 'beta' can be set
+The time frame of the synthesized data can be chosen, e.g. 4 days, 14 days, or 150 days
+	then a Kalman online regression is done with the synthesized data
+	at the end of the notebook the estimated regression parameters 'alpha' and 'beta' are compared with the set ones, the relative error is calculated
+	It is also shown that a classical offline multi linear regression and the online regression using a Kalman filter are resulting in identical predictions of the regression coefficients when working on the same set of synthesized data
+	by playing with different time frames 'number_of_days', it can easily be seen that the estimation accuracy improves with a longer time frame. Recommended minimum regression duration is about 14 days.
+ See also some additional comments at the end of the notebook
+ Please play with this Jupyter notebook to get a better understanding of the Kalman online linear regression