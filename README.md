# IsingMonteCarlo
A program implementing Metropolis Monte Carlo for the 2D square-lattice Ising model and the spin block renormalization.

[![CodeFactor](https://www.codefactor.io/repository/github/aromanro/isingmontecarlo/badge)](https://www.codefactor.io/repository/github/aromanro/isingmontecarlo)

A more detailed description can be found at https://compphys.go.ro/metropolis-for-2d-ising-and-spin-block-renormalization/

### PROGRAM IN ACTION

[![Program video](https://img.youtube.com/vi/gR8qr7IaS4I/0.jpg)](https://youtu.be/gR8qr7IaS4I)

### TOOLS

The project compiles on Windows with Visual Studio 2015.

### LIBRARIES

Besides mfc and other typical VC++ runtime libraries, the program uses GDI+ for drawing.

### CLASSES

Before presenting the classes, a little bit of a rant. I displayed the spins first with GDI+ calls - `FillRectangle`. I have a pretty decent monitor so it's quite a bit of resolution. With the application maximized, the frame rate achievable was very low. One would expect more from a dual Xeon workstation and a pretty decent video card (although with passive cooling). I 'downgraded' to simple GDI to learn that there is no much of a difference, which I expected, but I had to try... then I switched to Direct2D, although I hate using it with mfc because print preview does not work with it (I did use it in the electric field lines project, though, just for fun). It appeared to be faster, but not much faster. I did not time it until that point, so I cannot say what was the difference, but it was pretty small. I turned back at the plain old GDI and used a bitmap instead. More precisely, a memory <A href="https://en.wikipedia.org/wiki/BMP_file_format" target="blank_">device independent bitmap (DIB)</a>. You can get more speed by using a device dependent one (I did even that in the past) but it's not worth it. The big speed jump (as in from 2 frames/second to 50 frames/second or more) was due of avoiding calls into the slow library and implementing the equivalent call into my own class. Probably using device dependent bitmaps would make it a little bit faster and using Direct2D even faster, but just a little bit. Not worth it.

So, here are the classes in the order of importance (sort of):

`SpinMatrix` - More or less similar with what I presented in the previous post, but with the addition of pre-calculating the exponential values and with renormalization code added. It can be initialized at 'zero' temperature or infinite temperature. It's just the spin matrix with periodic boundary conditions, containing the implementation of a Metropolis Monte Carlo sweep on the spins.

`MonteCarloThread` - Has a `SpinMatrix` member called `spins`. In `Calculate` it runs a `TemperatureStep` for each temperature in the interval, starting with the lower one. Before this loop it runs a 'warmup' loop. The `TemperatureStep` just runs several sweeps for equilibration then several others for gathering statistics. The numbers are configurable, of course. One thread will pass the spin matrix to the main thread for displaying - in `PassData`, all threads will gather statistics in `PassStats`.

`Statistics` - Contains the accumulated statistics and has some operators overloaded and a `CollectStats` method. The usage is pretty straightforward.

`CIsingMonteCarloDoc` - The document class. Manages the threads, the data gathering and the spin matrices for display - both the ones that are displayed during threads running and when displaying the spin block renormalization. It also handles the charts and chart data. It has a copy of `Options` because the options that are stored in the application object can change during threads running.

`MemoryBitmap` - The class that helps with displaying the spin matrix. Plain old GDI for drawing, nothing fancy.

`CIsingMonteCarloView` - the view. Has a timer, deals with displaying and printing. Also holds some `MemoryBitmap` members for displaying.

`Chart` - The charting class. I just copied it from the nrg project. It uses GDI+ for drawing.

`Options` - The options class, it has methods to save them into registry and load them from registry.

`COptionsPropertySheet`, `CIsingModelPropertyPage`, `CSimulationPropertyPage`, `CRenormalizationPropertyPage`, `CDisplayPropertyPage`, `CChartsPropertyPage`, the property sheet class and the property pages classes, respectively. Quite standard mfc implementations, nothing hard to understand.

`CNumberEdit` - the edit control for double/float values. I just copied it from the nrg project.

`ComputationThread` - the base class for `MonteCarloThread`. 

`CMainFrame` - the main frame. Deals with menu commands.

`CIsingMonteCarloApp` - the application class. Has an `Option` member, loads the options at startup and also deals with GDI+ initialization.

`CAboutDlg` - what the name says.

