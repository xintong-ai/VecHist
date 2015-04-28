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


filename = np.arange(0.22, 1.000001, 0.01)
for scl in filename:
    hlistName = 'hlist_' + "{:.5f}".format(scl) + '.list'
    particleName = 'ds14_scivis_0128_e4_dt04_' + "{:.4f}".format(scl)
    outputName = "{:.5f}_".format(scl)
    outputPrefix = "C:\\GravityLabDataSet\\Universe\\loadFromRaw\\"
    outputName =  outputPrefix + outputName
    print hlistName
    print particleName
    print outputName

    prefix = "http://darksky.slac.stanford.edu/scivis2015/data/ds14_scivis_0128/"
    # Load N-body particles from a = 1.0 dataset. Particles have positions with
    # units of proper kpc, and velocities with units of km/s.
    particles = load_sdf(prefix+particleName)

    # Load the a=1 Rockstar hlist file. The header of the file lists the useful
    # units/information.
    scale, id, desc_scale, desc_id, num_prog, pid, upid, desc_pid, phantom, \
        sam_mvir, mvir, rvir, rs, vrms, mmp, scale_of_last_MM, vmax, x, y, z, \
        vx, vy, vz, Jx, Jy, Jz, Spin, Breadth_first_ID, Depth_first_ID, \
        Tree_root_ID, Orig_halo_ID, Snap_num, Next_coprogenitor_depthfirst_ID, \
        Last_progenitor_depthfirst_ID, Rs_Klypin, M_all, M200b, M200c, M500c, \
        M2500c, Xoff, Voff, Spin_Bullock, b_to_a, c_to_a, A_x, A_y, A_z, \
        b_to_a_500c, c_to_a_500c, A_x_500c, A_y_500c, A_z_500c, T_over_U, \
        M_pe_Behroozi, M_pe_Diemer, Macc, Mpeak, Vacc, Vpeak, Halfmass_Scale, \
        Acc_Rate_Inst, Acc_Rate_100Myr, Acc_Rate_Tdyn = \
        loadtxt(prefix+"rockstar/hlists/" + hlistName, unpack=True)



    # Now we want to convert the proper kpc of the particle position to comoving
    # Mpc/h, a common unit used in computational cosmology in general, but
    # specifically is used as the output unit in the merger tree halo list loaded
    # in above. First we get the Hubble parameter, here stored as 'h_100' in the
    # SDF parameters. Then we load the simulation width, L0, which is also in
    # proper kpc. Finally we load the scale factor, a, which for this particular
    # snapshot is equal to 1 since we are loading the final snapshot from the
    # simulation.
    h_100 = particles.parameters['h_100']
    width = particles.parameters['L0']
    cosmo_a = particles.parameters['a']
    kpc_to_Mpc = 1./1000
    sl = slice(0,None)

    # Define a simple function to convert proper to comoving Mpc/h.
    convert_to_cMpc = lambda proper: (proper + cosmo_a*width/2.) * h_100 * kpc_to_Mpc / cosmo_a
    print "cosmo_a: " + str(cosmo_a)


    haloid = np.array( id )
    fn = outputName + "id.npy"
    np.save( fn, haloid )
    halox = np.array( x )
    fn = outputName + "halox.npy"
    np.save( fn, halox )
    haloy = np.array( y )
    fn = outputName + "haloy.npy"
    np.save( fn, haloy )
    haloz = np.array( z )
    fn = outputName + "haloz.npy"
    np.save( fn, haloz )
    halorvir = np.array( rvir )
    fn = outputName + "halorvir.npy"
    np.save( fn, halorvir )
    halovx = np.array( vx )
    fn = outputName + "halovx.npy"
    np.save( fn, halovx )
    halovy = np.array( vy )
    fn = outputName + "halovy.npy"
    np.save( fn, halovy )
    halovz = np.array( vz )
    fn = outputName + "halovz.npy"
    np.save( fn, halovz )

    partx = np.array( convert_to_cMpc(particles['x'][sl]) )
    fn = outputName + "partx.npy"
    np.save( fn, partx )
    party = np.array( convert_to_cMpc(particles['y'][sl]) )
    fn = outputName + "party.npy"
    np.save( fn, party )
    partz = np.array( convert_to_cMpc(particles['z'][sl]) )
    fn = outputName + "partz.npy"
    np.save( fn, partz )
    partvx = np.array( convert_to_cMpc(particles['vx'][sl]) )
    fn = outputName + "partvx.npy"
    np.save( fn, partvx )
    partvy = np.array( convert_to_cMpc(particles['vy'][sl]) )
    fn = outputName + "partvy.npy"
    np.save( fn, partvy )
    partvz = np.array( convert_to_cMpc(particles['vz'][sl]) )
    fn = outputName + "partvz.npy"
    np.save( fn, partvz )




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