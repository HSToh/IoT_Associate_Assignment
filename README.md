# IoT_Associate_Assignment
# Disclaimer

The code files uploaded to this repository consists of that up to a single NodeMCU only. A single NodeMCU paired with a 16-channel analogue multiplexer module
is able to receive 16 analogue inputs.

Each motor to be monitored is assumed to be paired with 2 sensors (temperature & vibration) that will send data to via 2 analogue pins. Thus a single NodeMCU
is able to monitor the temperature and vibration parameters of 8 motors.

For the remaining 92 motors (total 100 motors to monitor as per requirement of the assignment question), further 12 NodeMCU modules will be required. For achieving this, the algorithm provided in this 
repository should be duplicated the same way.



# NodeMCU Code
Another assumption made is the data from the motor sensors are received by the NodeMCU in the correct format and does not require additional processing. The 
sensor values are published directly to the MQTT broker once received.

# Node-RED Dashboard Flow
The dashboard developed with Node-RED is used to display real-time sensor data representing motor conditions while performing data logging locally in CSV format.
The dashboard designed here is set locally on Windows OS. In reality, it should be hosted on a cloud platform.
