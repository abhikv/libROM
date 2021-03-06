/******************************************************************************
 *
 * Copyright (c) 2013-2017, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory
 * Written by William Arrighi wjarrighi@llnl.gov
 * CODE-686965
 * All rights reserved.
 *
 * This file is part of libROM.
 * For details, see https://computation.llnl.gov/librom
 * Please also read README_BSD_NOTICE.
 *
 * Redistribution and use in source and binary forms, with or without
 * modifications, are permitted provided that the following conditions are met:
 *
 *    o Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the disclaimer below.
 *    o Redistribution in binary form must reproduce the above copyright
 *      notice, this list of conditions and the disclaimer (as noted below) in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    o Neither the name of the LLNS/LLNL nor the names of its contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
 * LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OR SUCH DAMAGE.
 *
 *****************************************************************************/

// Description: The class that determines the next time at which a sample
//              should be taken for basis generation using an incremental SVD
//              approach.

#ifndef included_IncrementalSVDSampler_h
#define included_IncrementalSVDSampler_h

#include "SVDSampler.h"
#include "IncrementalSVD.h"
#include <boost/shared_ptr.hpp>

namespace CAROM {

/**
 * Class IncrementalSVDSampler knows, given an incremental svd implementation,
 * the time at which the next sample is needed.  It also knows given a time
 * whether it is time for the next sample.  There are two factors determining
 * if it is time for the next sample:
 * 1) the current time compared to the time the next sample must happen
 * 2) the number of time steps since the last sample
 */
class IncrementalSVDSampler : public SVDSampler
{
   public:
      /**
       * @brief Constructor.
       *
       * @pre dim > 0
       * @pre linearity_tol > 0.0
       * @pre initial_dt > 0.0
       * @pre samples_per_time_interval > 0
       * @pre sampling_tol > 0.0
       * @pre max_time_between_samples > 0.0
       * @pre min_sampling_time_step_scale >= 0.0
       * @pre sampling_time_step_scale >= 0.0
       * @pre max_sampling_time_step_scale >= 0.0
       * @pre min_sampling_time_step_scale <= max_sampling_time_step_scale
       *
       * @param[in] dim The dimension of the system on this processor.
       * @param[in] linearity_tol Tolerance to determine whether or not a
       *                          sample is linearly dependent.
       * @param[in] skip_linearly_dependent If true skip linearly dependent
       *                                    samples.
       * @param[in] fast_update If true use the fast update incremental svd
       *                        algorithm.
       * @param[in] initial_dt Initial simulation time step.
       * @param[in] samples_per_time_interval The maximum number of samples in
       *                                      each time interval.
       * @param[in] sampling_tol Sampling control tolerance.  Limits error in
       *                         projection of sample into reduced order space
       *                         followed by a lift back to full order space.
       * @param[in] max_time_between_samples Upper bound on time between
       *                                     samples.
       * @param[in] save_state If true the state of the SVD will be written to
       *                       disk when the object is deleted.  If there are
       *                       multiple time intervals then the state will not
       *                       be saved as restoring such a state makes no
       *                       sense.
       * @param[in] restore_state If true the state of the SVD will be restored
       *                          when the object is created.
       * @param[in] min_sampling_time_step_scale Minimum overall scale factor
       *                                         to apply to time step.
       * @param[in] sampling_time_step_scale Scale factor to apply to sampling
       *                                     algorithm.
       * @param[in] max_sampling_time_step_scale Maximum overall scale factor
       *                                         to apply to time step.
       * @param[in] debug_algorithm If true results of incremental svd
       *                            algorithm will be printed to facilitate
       *                            debugging.
       */
      IncrementalSVDSampler(
         int dim,
         double linearity_tol,
         bool skip_linearly_dependent,
         bool fast_update,
         double initial_dt,
         int samples_per_time_interval,
         double sampling_tol,
         double max_time_between_samples,
         bool save_state = false,
         bool restore_state = false,
         double min_sampling_time_step_scale = 0.1,
         double sampling_time_step_scale = 0.8,
         double max_sampling_time_step_scale = 5.0,
         bool debug_algorithm = false);

      /**
       * @brief Destructor.
       */
      ~IncrementalSVDSampler();

      /**
       * @brief Returns true if it is time for the next sample.
       *
       * @param[in] time Time of interest.
       *
       * @return True if it is time for the next sample to be taken.
       */
      virtual
      bool
      isNextSample(
         double time);

      /**
       * @brief Computes next time a sample is needed.
       *
       * @pre u_in != 0
       * @pre rhs_in != 0
       * @pre time >= 0.0
       *
       * @param[in] u_in The state at the specified time.
       * @param[in] rhs_in The right hand side at the specified time.
       * @param[in] time The simulation time for the state.
       */
      virtual
      double
      computeNextSampleTime(
         double* u_in,
         double* rhs_in,
         double time);

      /**
       * @brief Resets sample time step.
       *
       * @param[in] new_dt New value of sample time step.
       */
      virtual
      void
      resetDt(
         double new_dt);

   private:
      /**
       * @brief Unimplemented default constructor.
       */
      IncrementalSVDSampler();

      /**
       * @brief Unimplemented copy constructor.
       */
      IncrementalSVDSampler(
         const IncrementalSVDSampler& other);

      /**
       * @brief Unimplemented assignment operator.
       */
      IncrementalSVDSampler&
      operator = (
         const IncrementalSVDSampler& rhs);

      /**
       * @brief Sampling control tolerance.
       *
       * Limits error in projection of solution into the reduced order space.
       */
      double d_tol;

      /**
       * @brief Maximum time between samples.
       */
      double d_max_time_between_samples;

      /**
       * @brief Minimum sampling time step scale factor.
       */
      double d_min_sampling_time_step_scale;

      /**
       * @brief Sampling time step scale factor to apply to algorithm.
       */
      double d_sampling_time_step_scale;

      /**
       * @brief Maximum sampling time step scale factor.
       */
      double d_max_sampling_time_step_scale;

      /**
       * @brief Current time step.
       */
      double d_dt;

      /**
       * @brief Next time at which a sample should be taken.
       */
      double d_next_sample_time;

      /**
       * @brief The number of processors being run on.
       */
      int d_num_procs;
};

}

#endif
