# Final Masters Project
## Master's Thesis
## Real-Time Multiple Fluid Simulation for Games
Jacob Worgan, s5107963
<p>
27/10/2022
</p>
Source code for this project is located in the ./Source/ folder. 
<br>
Simulation effects are located in the ./Content/Effects/ folder. The main effects worked on are P_SPHTest1 (testing particle-based fluid simulation), P_FluidTest1 (testing grid-based fluid simulation), and P_InOutTest (testing data input and output for Niagara effects).
<br>
Further effects found in the /UnrealExampleFluids/ folder are copies of Unreal Engine 5's Niagara Fluids effects used as reference, and are not made by myself. 

___
### Tests
A range of tests have been written to aid in the development of this project, using Unreal's Automation Testing system. These can be run by selecting /Window/Testing/Test Automation/ on the top bar in the Unreal Editor. Select the application on the left, then the automation tab on the right section of the screen. You can then select and run tests. The tests I have written for this project are under 'CharPlayer', 'InputOutput2', 'WaterBallProj', and 'WaterSpout'. Try to only run a couple of tests at a time, as running too many tests may cause the editor to not respond.

___
### Controls
<p>
Keyboard:
<br>
Control the player with WASD keys, jump with the space bar, and absorb/release with q and e keys respectively. 
</p>

<p>
Gamepad:
<br>
Control the player with the control stick, right face button to jump, and left/right triggers to absorb/release respectively. 
</p>