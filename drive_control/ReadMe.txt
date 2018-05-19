Inputs: Double angle

defined parameters: 
	TURN_MARGIN -> PICO should not only start turning for angles bigger than the 			TURN_MARGIN
	TURN_COMPLETE -> PICO's is done turning when it the current angle is the 			desired	angle +- TURN_COMPLETE

Functionalities:
	PICO starts turning to the desired angle, then it starts moving forwards.
	When the desired angle < TURN_MARGIN PICO starts driving sideways in that angle 		without turning.

Outputs:
	None

