#ifndef included_incremental_svd_rom_h
#define included_incremental_svd_rom_h

#include "svd_rom.h"
#include "incremental_svd_time_stepper.h"

namespace CAROM {

// A class which implements a Reduced Order Model based on the incremental svd
// algorithm.
  class incremental_svd_rom : public svd_rom
{
   public:
      // Constructor.
      incremental_svd_rom(
         int dim,
         double epsilon,
         bool skip_redundant,
         int rom_size,
         double tolerance,
         double max_time_between_snapshots,
         bool fast_update);

      // Destructor.
      virtual
      ~incremental_svd_rom();

      // Returns true if it is time for the next svd snapshot.
      virtual
      bool
      isNextSnapshot(
         double time);

      // Add a snapshot to the incremental svd at the given time.
      virtual
      void
      takeSnapshot(
         double* u_in,
         double time);

      // Computes next time an svd snapshot is needed.
      virtual
      double
      computeNextSnapshotTime(
         double* u_in,
         double* rhs_in,
         double time);

      // Returns the basis vectors at the given time as a Matrix.
      virtual
      const Matrix*
      getBasis(
         double time);

      // Returns the number of time intervals on which different sets of basis
      // vectors are defined.
      virtual
      int
      getNumBasisTimeIntervals() const;

      // Returns the start time for the requested time interval.
      virtual
      double
      getBasisIntervalStartTime(
         int which_interval) const;

   private:
      // Unimplemented default constructor.
      incremental_svd_rom();

      // Unimplemented copy constructor.
      incremental_svd_rom(
         const incremental_svd_rom& other);

      // Unimplemented assignment operator.
      incremental_svd_rom&
      operator = (
         const incremental_svd_rom& rhs);

      // The underlying time step control object.
      boost::shared_ptr<incremental_svd_time_stepper> d_isvdts;
};

}

#endif
