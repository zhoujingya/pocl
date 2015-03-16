/* pocl_llvm.h: interface to call LLVM and Clang.

   Copyright (c) 2013 Kalle Raiskila and
                      Pekka Jääskeläinen
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#pragma once
#include "pocl_cl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Compiles an .cl file into LLVM IR.
 */
int pocl_llvm_build_program
(cl_program program,
 cl_device_id device,
 int device_i,     
 const char* cache_dir,
 const char* binary_filename,
 const char* device_tmpdir,
 const char* user_options,
 int fd);


/* Retrieve metadata of the given kernel in the program to populate the
 * cl_kernel object.
 */
int pocl_llvm_get_kernel_metadata
(cl_program program, 
 cl_kernel kernel,
 int device_i,     
 const char* kernel_name,
 const char* device_tmpdir, 
 char* descriptor_filename,
 int *errcode);

/* This function links the input kernel LLVM bitcode and the
 * OpenCL kernel runtime library into one LLVM module, then
 * runs pocl's kernel compiler passes on that module to produce 
 * a function that executes all work-items in a work-group.
 *
 * Output is a LLVM bitcode file that contains a work-group function
 * and its associated launchers. 
 *
 * TODO: this is not thread-safe, it changes the LLVM global options to
 * control the compilation. We should enforce only one compilations is done
 * at a time or control the options through thread safe methods.
 */
int pocl_llvm_generate_workgroup_function
(cl_device_id device,
 cl_kernel kernel,
 size_t local_x, size_t local_y, size_t local_z,
 const char* parallel_filename,
 const char* kernel_filename);

/**
 * Update the program->binaries[] representation of the kernels
 * from the program->llvm_irs[] representation.
 * Also updates the 'program.bc' file in the POCL_TEMP_DIR cache.
 */
void pocl_llvm_update_binaries (cl_program program);

/**
 * Find the "__kernel" function names in 'program',
 * filling the callee-allocated array with pointer to the program binary.
 * No more than 'max_num_krn' are written.
 *
 * Results are valid as long as program binary is not modified.
 *
 * Returns the number of kernels found in the program (may be greater than
 * 'max_num_krn')
 */
unsigned pocl_llvm_get_kernel_names( cl_program program, const char **knames, unsigned max_num_krn);

/** Compile the kernel in infile from LLVM bitcode to native object file for
 * device, into outfile.
 */
int pocl_llvm_codegen ( cl_kernel kernel,
                        cl_device_id device,
                        const char *infile,
                        const char *outfile);

/* Parse program file and populate program's llvm_irs */
void
pocl_update_program_llvm_irs(cl_program program,
                       cl_device_id device, const char* program_filename);


/* Remove a directory, recursively */
int pocl_rm_rf(const char* path);

/* Make a directory, including all directories along path */
int pocl_mkdir_p(const char* path);

/* Remove a file or empty directory */
int pocl_remove(const char* path);

int pocl_exists(const char* path);

int pocl_filesize(const char* path, uint64_t* res);

/* Writes or appends data to a file */
int pocl_write_file(const char* path, const char* content_dptr,
                    size_t count, int append, int dont_rewrite);

/* Allocates memory and places file contents in it. Returns number of chars read */
int pocl_read_file(const char* path, char* content_dptr, uint64_t read_bytes);

/* Touch file to change last modified time. For portability, this
 * removes & creates the file. Thus its not atomic. */
int pocl_touch_file(const char* path);


#ifdef __cplusplus
}
#endif
