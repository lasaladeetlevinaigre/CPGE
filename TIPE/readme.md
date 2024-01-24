
TIPE



The use of the knowledge of the displacement of an object in successive frames is called Motion Compensation.






https://www.maketecheasier.com/how-video-compression-works/



https://aws.amazon.com/blogs/media/back-to-basic-what-mechanisms-are-used-behind-the-scenes-in-video-compression/

Because there are fewer B and P frames in the top example, donc plus de I frames, the encoder has to quantize the I-frames more coarsely (compress them more) to fit within the configured bitrate, which results in blockiness, blurriness, and loss of detail.



University of Texas
https://arxiv.org/pdf/1811.10673.pdf



https://arxiv.org/pdf/1812.00101v3.pdf



https://arxiv.org/pdf/2003.01966.pdf
Compresse différent niveaux, frames devinées de plus basses résol, les frames de red HD



https://aws.amazon.com/blogs/media/part-1-back-to-basics-gops-explained/
Importance of I frame pas trop éloignées




https://ottverse.com/discrete-cosine-transform-dct-video-compression/


https://eprints.nottingham.ac.uk/13447/1/thesis.pdf
https://books.google.fr/books?id=e7xnBwAAQBAJ&pg=PA364&redir_esc=y#v=onepage&q&f=false
https://books.google.fr/books?id=7XuU8T3ooOAC&pg=PA1&redir_esc=y#v=onepage&q&f=false


[LIBRAIRIES C]
ffmpeg - libavcodec












The  main  disadvantage  of  block  motion  compensation  is  that  it  introduces discontinuities at the block borders (blocking artifacts). These artifacts appear in the form of sharp horizontal and vertical edges which are easily spotted by the human eye and produce ringing effects (large coefficients in high frequency sub-bands) in the Fourier-related transform used for transform coding of the residual frames.
https://www.researchgate.net/publication/301230297_A_Hardware_implementation_of_true-motion_estimation_with_3-D_recursive_search_block_matching_algorithm


Different algo de blockmatching
	- 3 Step search
	- 2D Logarithmic
https://course.ece.cmu.edu/~ee899/project/deepak_mid.htm