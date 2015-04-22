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

#load data
fn = "halox.npy"
halox = np.load( fn )
fn = "haloy.npy"
haloy = np.load( fn )
fn = "haloz.npy"
haloz = np.load( fn )
fn = "halorvir.npy"
halorvir = np.load( fn )

fn = "partx.npy"
partx = np.load( fn )
fn = "party.npy"
party = np.load( fn )
fn = "partz.npy"
partz = np.load( fn )
fn = "partvx.npy"
partvx = np.load( fn )
fn = "partvy.npy"
partvy = np.load( fn )
fn = "partvz.npy"
partvz = np.load( fn )




#load data, only use the parameters
prefix = "http://darksky.slac.stanford.edu/scivis2015/data/ds14_scivis_0128/"
particles = load_sdf(prefix+"ds14_scivis_0128_e4_dt04_1.0000")

h_100 = particles.parameters['h_100']
width = particles.parameters['L0']
cosmo_a = particles.parameters['a']
kpc_to_Mpc = 1./1000

convert_to_cMpc = lambda proper: (proper) * h_100 * kpc_to_Mpc / cosmo_a
halorvir = convert_to_cMpc( halorvir )

halosize = halox.size

pl.figure(figsize=[10,10])
pl.scatter(partx, party, color='r', s=1.0, alpha=0.05)

partVelocity = np.zeros( [partx.size, 3] )
numPart = np.zeros( halosize )

partCnt = 0
for i in range(0,500):
    print i
    print partCnt
    x = halox[i]
    y = haloy[i]
    z = haloz[i]
    rvir = halorvir[i]

    d = np.sqrt( np.power( ( partx - x ), 2 ) + np.power( ( party - y ), 2 ) + np.power( ( partz - z ), 2 ) )

    result = np.array( np.where( d<rvir ) )
    result = np.reshape( result, result.size )

    #pl.scatter(partx[result], party[result], color=cm.jet(i/float(500)), s=1.0, alpha=0.05)

    # for j in range( 0, result.size ):
    #     partVelocity[ partCnt, : ] = np.array( [ partvx[result[j]], partvy[result[j]], partvz[result[j]] ] )
    #     partCnt += 1

    partCnt += result.size
    numPart[i] = result.size

print partCnt
print numPart
print partVelocity

fn = "velocity.npy"
np.save( fn, partVelocity[ 0:partCnt, :] )

fn = "particlesInHalo.npy"
np.save( fn, numPart )


print "dont"
pl.savefig("halos_and_particles.png", bbox_inches='tight')
print "2222"

# prefix = "http://darksky.slac.stanford.edu/scivis2015/data/ds14_scivis_0128/"
# # Load N-body particles from a = 1.0 dataset. Particles have positions with
# # units of proper kpc, and velocities with units of km/s.
# particles = load_sdf(prefix+"ds14_scivis_0128_e4_dt04_1.0000")
#
# # Load the a=1 Rockstar hlist file. The header of the file lists the useful
# # units/information.
# scale, id, desc_scale, desc_id, num_prog, pid, upid, desc_pid, phantom, \
#     sam_mvir, mvir, rvir, rs, vrms, mmp, scale_of_last_MM, vmax, x, y, z, \
#     vx, vy, vz, Jx, Jy, Jz, Spin, Breadth_first_ID, Depth_first_ID, \
#     Tree_root_ID, Orig_halo_ID, Snap_num, Next_coprogenitor_depthfirst_ID, \
#     Last_progenitor_depthfirst_ID, Rs_Klypin, M_all, M200b, M200c, M500c, \
#     M2500c, Xoff, Voff, Spin_Bullock, b_to_a, c_to_a, A_x, A_y, A_z, \
#     b_to_a_500c, c_to_a_500c, A_x_500c, A_y_500c, A_z_500c, T_over_U, \
#     M_pe_Behroozi, M_pe_Diemer, Macc, Mpeak, Vacc, Vpeak, Halfmass_Scale, \
#     Acc_Rate_Inst, Acc_Rate_100Myr, Acc_Rate_Tdyn = \
#     loadtxt(prefix+"rockstar/hlists/hlist_1.00000.list", unpack=True)
#
#
#
# # Now we want to convert the proper kpc of the particle position to comoving
# # Mpc/h, a common unit used in computational cosmology in general, but
# # specifically is used as the output unit in the merger tree halo list loaded
# # in above. First we get the Hubble parameter, here stored as 'h_100' in the
# # SDF parameters. Then we load the simulation width, L0, which is also in
# # proper kpc. Finally we load the scale factor, a, which for this particular
# # snapshot is equal to 1 since we are loading the final snapshot from the
# # simulation.
# h_100 = particles.parameters['h_100']
# width = particles.parameters['L0']
# cosmo_a = particles.parameters['a']
# kpc_to_Mpc = 1./1000
# sl = slice(0,None)
#
# # Define a simple function to convert proper to comoving Mpc/h.
# convert_to_cMpc = lambda proper: (proper + width/2.) * h_100 * kpc_to_Mpc / cosmo_a
#
# halox = np.array( x )
# fn = "halox.npy"
# np.save( fn, halox )
# haloy = np.array( y )
# fn = "haloy.npy"
# np.save( fn, haloy )
# haloz = np.array( z )
# fn = "haloz.npy"
# np.save( fn, haloz )
# halorvir = np.array( rvir )
# fn = "halorvir.npy"
# np.save( fn, halorvir )
#
#
# partx = np.array( convert_to_cMpc(particles['x'][sl]) )
# fn = "partx.npy"
# np.save( fn, partx )
# party = np.array( convert_to_cMpc(particles['y'][sl]) )
# fn = "party.npy"
# np.save( fn, party )
# partz = np.array( convert_to_cMpc(particles['z'][sl]) )
# fn = "partz.npy"
# np.save( fn, partz )
# partvx = np.array( convert_to_cMpc(particles['vx'][sl]) )
# fn = "partvx.npy"
# np.save( fn, partvx )
# partvy = np.array( convert_to_cMpc(particles['vy'][sl]) )
# fn = "partvy.npy"
# np.save( fn, partvy )
# partvz = np.array( convert_to_cMpc(particles['vz'][sl]) )
# fn = "partvz.npy"
# np.save( fn, partvz )

#loading
# fn = "halox.npy"
# hh = np.load( fn )
# print hh


#
# # Plot all the particles, adding a bit of alpha so that we see the density of
# # points.
# import matplotlib.pylab as pl
# pl.figure(figsize=[10,10])
#
# pl.scatter(convert_to_cMpc(particles['x'][sl]),
#            convert_to_cMpc(particles['y'][sl]), color='b', s=1.0, alpha=0.05)
#
# # Plot all the halos in red.
# pl.scatter(x, y, color='r', alpha=0.1)
#
# # Add some labels
# pl.xlabel('x [cMpc/h]')
# pl.ylabel('y [cMpc/h]')
# pl.savefig("halos_and_particles.png", bbox_inches='tight')
#
# # Could now consider coloring halos by any of the various quantities above.
# # Perhaps mvir would be nice to show the virial Mass of the halo, or we could
# # scale the points by the virial radius, rvir.