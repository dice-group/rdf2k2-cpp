# rdf2k2-cpp
RDF Compression using k2-trees in C++

Compresses an RDF file to a k2 tree with a HDT Dictionary. Will result into two files DATASET.k2 and DATASET.k2.dict.

## Java

For the java version visit https://github.com/dice-group/GraphCompression

# Build

```
./build.sh
```
The executable will be in build_release/rdf2k2_cpp 

# Usage

## Compression

```
rdf2k2_cpp -c -tkd2 INPUT_RDF OUTPUT_K2.2
```

## Decompression
You need the INPUT_K2k2 file and the INPUT_K2k2.dict file in the same folder

```
rdf2k2_cpp -d -kd2 INPUT_K2.k2 OUTPUT_RDF_NTRIPLE
```

# Precompressed Datasets

You can get precompressed datasets at https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/

* [ArchivesHub](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/archiveshub/)
* [Jamendo](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/jamendo/)
* [scholarly data](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/scholarlydata/)
* [DBLP 2017](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/dblp-2017/)
* [DBpedia en 03.2021 in sections](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/dbpedia-03_2021/dbpedia-sections/)
* [DBpedia en 03.2021 single file](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/dbpedia-03_2021/)
* [DBpedia all other available languages without nif datasets 10.2016](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/dbpedia-10_2016/)
* [DBpedia all other available languages with nif datasets 10.2016](https://hobbitdata.informatik.uni-leipzig.de/rdf2k2/dbpedia-10_2016-nif/)


# Troubleshooting
TODO
