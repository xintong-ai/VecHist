# ./examples/load_data.py
# Required libraries:
# pip install sdfpy
# pip install thingking

from sdfpy import load_sdf
from thingking import loadtxt
import numpy as np
import math
import matplotlib.cm as cm
import matplotlib.pylab as pl


gminx = 100000
gmaxx =-100000
gminy = 100000
gmaxy =-100000
gminz = 100000
gmaxz =-100000

#filename = np.arange(0.12, 1.000001, 0.01)
filename = np.arange(1.0, 1.000001, 0.01)
for scl in filename:
    filePrefix = 'C:\\GravityLabDataSet\\Universe\\loadFromRaw\\'
    particleName = 'ds14_scivis_0128_e4_dt04_' + "{:.4f}".format(scl)
    outputName = "{:.5f}_".format(scl)
    print outputName

    #load data
    fn = filePrefix + outputName + "partx.npy"
    partx = np.load( fn )
    fn = filePrefix + outputName + "party.npy"
    party = np.load( fn )
    fn = filePrefix + outputName + "partz.npy"
    partz = np.load( fn )

    dim = 500.0
    skip = 62.5/dim
    density = np.zeros( [int(dim),  int(dim),  int(dim)] )
    for i in range(0, partx.size):
        if i % 10000 == 0:
            print i
        xidx = math.floor( partx[i]/skip )
        yidx = math.floor( party[i]/skip )
        zidx = math.floor( partz[i]/skip )
        if xidx >= dim:
            xidx -= 1
        if yidx >= dim:
            yidx -= 1
        if zidx >= dim:
            zidx -= 1
        density[xidx, yidx, zidx] += 1
    #
    # xspace = np.arange(0, 62.5, skip)
    # yspace = np.arange(0, 62.5, skip)
    # zspace = np.arange(0, 62.5, skip)
    # cnt = 0
    # for x in xspace:
    #     print "x: "+ str(x)
    #     for y in yspace:
    #         for z in zspace:
    #             xand = np.logical_and( x <= partx, partx <x+skip )
    #             yand = np.logical_and( y <= party, party <y+skip )
    #             zand = np.logical_and( z <= partz, partz <z+skip )
    #
    #             xyand = np.logical_and( xand, yand )
    #             xyzand = np.logical_and( xyand, zand )
    #
    #             density[cnt] = np.array( np.where( xyzand ) ).size
    #             cnt += 1

    density = np.reshape(density, density.size)
    max = np.amax( density )
    density /= float( max )
    max = np.amax( density )
    density.astype('float32').tofile('density.bin')
    pause


    # fn = filePrefix + outputName + "halorvir.npy"
    # halorvir = np.load( fn )
    #


    #load data, only use the parameters
    # prefix = "http://darksky.slac.stanford.edu/scivis2015/data/ds14_scivis_0128/"
    # particles = load_sdf(prefix+particleName)
    #
    # # h_100 = particles.parameters['h_100']
    # # width = particles.parameters['L0']
    # # cosmo_a = particles.parameters['a']
    # # kpc_to_Mpc = 1./1000
    # #
    # # convert_to_cMpc = lambda proper: (proper) * h_100 * kpc_to_Mpc / cosmo_a
    # # halorvir = convert_to_cMpc( halorvir )

    # halosize = halox.size
    #
    # pl.figure(figsize=[10,10])
    # pl.scatter(partx, party, color='r', s=1.0, alpha=0.05)
    #
    # partVelocity = np.zeros( [partx.size, 3] )
    # numPart = np.zeros( halosize )
    #
    # partCnt = 0
    for i in range(0,halosize):
        print i
        print partCnt
        x = halox[i]
        y = haloy[i]
        z = haloz[i]
        rvir = halorvir[i]

        d = np.sqrt( np.power( ( partx - x ), 2 ) + np.power( ( party - y ), 2 ) + np.power( ( partz - z ), 2 ) )

        result = np.array( np.where( d<rvir ) )
        result = np.reshape( result, result.size )

        pl.scatter(partx[result], party[result], color=cm.jet(0), s=1.0, alpha=0.05)

        for j in range( 0, result.size ):
            partVelocity[ partCnt, : ] = np.array( [ partvx[result[j]], partvy[result[j]], partvz[result[j]] ] )
            partCnt += 1

        #partCnt += result.size
        numPart[i] = result.size

    print partCnt
    print numPart

    np.savetxt('vv.txt', partVelocity[0:partCnt,:], newline=" \n")
    pause


    outputFilePrefix = 'C:\\GravityLabDataSet\\Universe\\collectVelocity\\'
    fn = outputFilePrefix + outputName + "velocity.npy"
    np.save( fn, partVelocity[ 0:partCnt, :] )

    fn = outputFilePrefix + outputName + "particlesInHalo.npy"
    np.save( fn, numPart )

    pl.savefig( outputFilePrefix + outputName + "halos_and_particles.png", bbox_inches='tight')

