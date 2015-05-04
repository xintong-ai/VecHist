import numpy as np
from sdfpy import load_sdf
cubemap_size = 16

gminx = 100000
gmaxx =-100000
gminy = 100000
gmaxy =-100000
gminz = 100000
gmaxz =-100000

scl = 0.22

filePrefix = 'C:\\GravityLabDataSet\\Universe\\loadFromRaw\\'
particleName = 'ds14_scivis_0128_e4_dt04_' + "{:.4f}".format(scl)
outputName = "{:.5f}_".format(scl)
print outputName

#load data
fn = filePrefix + outputName + "id.npy"
haloid = np.load( fn )
fn = filePrefix + outputName + "halox.npy"
halox = np.load( fn )
fn = filePrefix + outputName + "haloy.npy"
haloy = np.load( fn )
fn = filePrefix + outputName + "haloz.npy"
haloz = np.load( fn )
fn = filePrefix + outputName + "halorvir.npy"
halorvir = np.load( fn )

fn = filePrefix + outputName + "partx.npy"
partx = np.load( fn )
fn = filePrefix + outputName + "party.npy"
party = np.load( fn )
fn = filePrefix + outputName + "partz.npy"
partz = np.load( fn )
fn = filePrefix + outputName + "partvx.npy"
partvx = np.load( fn )
fn = filePrefix + outputName + "partvy.npy"
partvy = np.load( fn )
fn = filePrefix + outputName + "partvz.npy"
partvz = np.load( fn )

gminx = np.amin(partx)
gmaxx = np.amax(partx)
gminy = np.amin(party)
gmaxy = np.amax(party)
gminz = np.amin(partz)
gmaxz = np.amax(partz)

print gminx
print gmaxx
print gminy
print gmaxy
print gminz
print gmaxz

#load data, only use the parameters
prefix = "http://darksky.slac.stanford.edu/scivis2015/data/ds14_scivis_0128/"
particles = load_sdf(prefix+particleName)

h_100 = particles.parameters['h_100']
width = particles.parameters['L0']
cosmo_a = particles.parameters['a']
kpc_to_Mpc = 1./1000

convert_to_cMpc = lambda proper: (proper) * h_100 * kpc_to_Mpc / cosmo_a
#halorvir = convert_to_cMpc( halorvir )

print h_100
print kpc_to_Mpc
print cosmo_a

halosize = halox.size

numPart = np.zeros( halosize )
optData = np.zeros( (halosize, 17 + 1536) )
print("num of halo: " + str(halosize))

partCnt = 0

############################
i = 0
id = haloid[i]
x = halox[i]
y = haloy[i]
z = haloz[i]
rvir = halorvir[i]

print rvir