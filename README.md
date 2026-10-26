# ann search lsh hypercube kmeans clustering cpp

## Assignment Description

The goal of the assignment is to **search** and **cluster** high-dimensional vectors efficiently. The vectors are images of handwritten digits 
from the **MNIST** dataset: each 28x28 image becomes a vector of 784 dimensions. All distances are computed with the **Euclidean metric (L2)**.

### Part A: Approximate Nearest Neighbor (ANN) search

Given a query image *q*, the programs `lsh` and `cube` return approximately:
- the **nearest neighbor** of *q*
- the **N nearest neighbors** of *q*
- all the images within **radius R** of *q* (**range search**)

Two algorithms are used:
- **LSH (Locality-Sensitive Hashing):** hash functions place nearby vectors in the same bucket with high probability, so a query is compared 
only with the vectors in its buckets across **L** hash tables
- **Random Projection on the Hypercube:** each vector is mapped to a vertex of a **k**-dimensional binary hypercube and the search checks the 
query's vertex and its nearby vertices (**probes**), up to **M** candidate points

For comparison, an **exhaustive (brute-force) search** finds the true nearest neighbors and the output reports the distances and search times 
of both methods.

## How it works

### LSH (Locality-Sensitive Hashing)
Normal hash functions scatter similar items randomly. LSH does the opposite, it uses hash functions designed so that nearby points are likely 
to land in the same bucket. Each point is projected onto random directions and cut into windows of width `w`, combining `k` such hashes gives 
one hash table and we build `L` tables. To answer a query, we only compare it against the points in its own buckets instead of the whole 
dataset, which is extremely much more faster at the cost of occasionally missing the true nearest neighbor.

### Hypercube (Random Projection)
Each point is mapped to a corner (vertex) of a `k`-dimensional binary cube: every LSH hash value is randomly but consistently turned into a 0 
or 1 (the same value always gets the same bit), giving a bit string like `0110…`. Similar points tend to get similar bit strings. To search, we 
look at the query's vertex first, then at neighboring vertices with small Hamming distance (1 bit different, then 2, …), checking at most `M` 
points and `probes` vertices.

### Why approximate?
Exact nearest-neighbor search in high dimensions (784 for MNIST) is essentially as slow as checking every point (the "curse of 
dimensionality"). ANN methods trade a little accuracy for a big speed-up, and the program reports both the approximate and the true 
(brute-force) results so you can compare them. The same structures can also speed up clustering, by using range search in the assignment step 
(reverse search).

### Part B: Clustering

The program `cluster` groups the images into **K** clusters with **k-means**:
- **Initialization:** **k-Means++** chooses initial centroids that are spread out
- **Assignment:** each image is assigned to its nearest centroid with the exact **Lloyd's** algorithm. The assignment also describes two alternatives: **reverse search** through range search with **LSH** or with the **Hypercube**
- **Update:** centroids are recalculated with the **MacQueen** method

The quality of the clustering is evaluated with the **Silhouette** coefficient.

---

<u>**Compilation and Execution of the program:**</u>

**A) If you want to run the cube:**

```
1) Inside the folder, type: make cube

2) You can type:

./cube

./cube -d <input file> -q <query file> -k <int> -M <int> -probes
<int> -o <output file> -N <number of nearest> -R <radius>

./cube -q <query file> -d <input file> -k <int> -M <int>
-R <radius> -probes  <int> -o <output file> -N <number of nearest>
(or any other combination you want)

./cube -q <query file> -d <input file>
(the rest of the parameters will take their default values and
 any other combination you want)
```

**B) If you want to run the lsh:**

```
1) Inside the folder, type: make lsh

2) You can type:

./lsh

./lsh -d <input file> -q <query file> -k <int> -L <int> -o <output
file> -N <number of nearest> -R <radius>

./lsh -d <input file> -q <query file> -k <int> -R <radius> -o
<output file> -N <number of nearest> -L <int>
(or any other combination you want)

./lsh -q <query file> -d <input file>
(the rest of the parameters will take their default values and
 any other combination you want)
```

**C) If you want to run part B of the assignment option (a):**

```
1) Inside the folder, type: make cluster

2) You can type:

./cluster -i <input file> -c <configuration file> -o <output file>
-complete <optional> -m <Classic>
(or any other combination you want)
```

<u>**Design choices and Explanation Implementation in exersise:**</u>

**1)** In the files Point.hpp/.cpp I implement the class Point which will represent the point of the assignment, of course with its corresponding private variables and various public functions through which we will be able to interact with it. <u>**Attention**</u> at the beginning I had defined a vector\<uint8_t\> coordinates for the representation of the coordinates of a point but then when I was in part B of the assignment and at some given moment I had some centroids and I was in the phase where I checked with some conditions if I need to go and find the next centroid this loop given that we have uint8_t was “gnawing away at” the coordinates of the centroid little by little so then the point 0 came out as the centroid which is obviously wrong, so to avoid making a new class Point2 exclusively for the centroid I put in the class Point the coordinates as vector\<double\> so with this change I am fine.

**2)** In the files min_Point.hpp/.cpp I implement the class min_point which will represent the minimum point of the assignment, defining appropriate variables for the representation of this structure as well as public functions so that the user can interact with this class.

**3)** In the files typical_functions.hpp/.cpp I define various typical functions, such as the function read_command(…) which reads and stores the parameters in the case that the user gives them on the command line for the cube, a similar function for the lsh is read_command_lsh(…) and I define the function read_binary_file(…) which reads data from a binary_file that we pass as a parameter and returns a structure vector\<Point\> which corresponds to the whole space on which we will work.

**4)** Initially in the file common_functions.hpp I define the class V_random_vector which will represent the normal (Gaussian) distribution (see theory page 20 pdf) and the corresponding functions that are required for the implementation of the above I define them in the file common_functions.cpp. **Among the functions from this file that need an explanation is the f_i(Point \*p, int h_p, map\<int, string\> \*f) it checks if it has mapped the key value of the value of h_p to 0 or 1 if not it adds it to the map and updates the f_string of some Point** (of course we will use this function in the cube later).

**5) So in the files cube.hpp/.cpp and main_cube.cpp I implement the structure of the cube. The most important parts that need explanation are : in the file main_cube.cpp** (which is the main of the cube) **in lines 53-54 I define a vector\<int\> h_p_i[k] where we have k tables vector\<int\> where at position I of the tables we refer to the I-th point, so the table j at position I has the value of h\_j\_(p\_I\_) and using the map\<int, string\> f placing the k values of 0 and 1 for the I-th point from the I positions of the tables we will find the name of the bucket in which we must put this point. The variable move (code line 60) is nothing other than the minimum value of an hpi which later I will use as normalize so that the h(p) come out positive i.e. what the theory says on page 19 of the pdf (i.e. move will certainly be a large negative value so -move will be a large positive value). Then in lines 88-100 I build the hyper_cube according to the theory and I call the function searching_part_cube(…) to implement the searches of the assignment.**

**6)** As for the part of the assignment for the lsh I apply exactly the theory pages 12-13, 16, 21 from the pdf and based on the logic I followed above initially I use various common functions with the cube or I modify them slightly to be compatible with the part of the lsh and in general I follow the same logic that I applied in the cube so I recommend that you first look at the cube to understand my logic and then come and look at the lsh so that the logic I apply is more familiar to you and so that I do not rewrite what I wrote in the cube but now slightly different for the lsh and the explanation process becomes more verbose and tiring

**7) The files cluster_functions.hpp/.cpp and cluster_main.cpp** I use them to implement from **part B of the assignment** the initialization with k-Means++, the update based on the MacQueen method and the algorithm of Lloyds. **What needs a justification is: initially in the file cluster_functions.hpp I define the class centroid which uses the point to represent the coordinates of the centroid and ATTENTION I understand the difference between the centroid and the structure of a cluster to which it will belong according to the theory of the course, but I perceived-interacted with the “space” of the assignment on the subject of the centroids as follows that every point will have a centroid which will be the nearest to it so a centroid is defined by the points that have it as nearest compared to other centroids of the space so for this reason I have map\<unsigned int, Point\> P where I map by key value the id of a point to this point in a specific centroid and the mean_point which will keep at a given moment the coordinates of the next centroid from the one we are at now.** So in the **file cluster_functions.cpp** besides defining some basic functions that are needed for part B of the assignment and others to interact with the class centroid **I build the function kMeanspp_centroids_initialization(…) which implements the theory pages 43-44 in the pdf but I also implement another one based on the theory of the course I implement my “own” function the my_method_centroids_initialization(…) which does exactly what the kMeanspp does but I reduce the complexity of the code(i.e. I do not need to search for every centroid the D(i), then to compute the P(r), to take a random x to choose the next centroid i.e. you see how much the complexity of the program increases (how many for loops we will have inside) and consequently the execution time) but based on the probabilistic nature of the course (i.e. we are not interested in choosing the best possible point but a point that is not bad) I define using #define a minimum distance min_distance that the centroids must have and I choose randomly k points in the space that are at least min_distance apart to become centroids. So these points may not necessarily be the farthest but they will be far, however we will have reduced the complexity of the algorithm and therefore the execution time lines 106-164 file cluster_functions.cpp.**\
**The logic of the update through MacQueen** (page 50 theory pdf) **is implemented in the function void MacQueen(…) where each time for each centroid we will compute the mean_point (the next centroid of this centroid) based on the points of the space that have this centroid as nearest at that given moment, so we will take as the next centroid this mean_point which is computed based on the points that had the previous centroid as nearest centroid** until at least one of the two conditions that I have inside the code no longer holds.\
Finally the assignment with the exact algorithm of Lloyds (page 32 from the theory pdf) is implemented above in the function assigment_Lloyd_method(…) where for each point we find its new nearest centroid and we erase it from the previous centroid and we put it in the map of the new centroid.

<u>**WHAT HAS BEEN IMPLEMENTED:**</u>

**EVERYTHING HAS BEEN IMPLEMENTED EXCEPT FROM PART B the reverse search through range search with (b)LSH or (c)Hyper_cube and the Silhouette**

**THANK YOU!!!!!!!**
