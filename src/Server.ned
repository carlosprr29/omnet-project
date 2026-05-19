//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package src;
simple Server 
{
    
    parameters:
    	@display("i=block/server");
        @signal[delay1](type="simtime_t");
        @statistic[residenceTime1](title="Residence Time Class 1"; source="delay1"; record=mean, vector);
        
        @signal[throughput1](type="long");
        @statistic[throughput1](title="Throughput Class 1"; source="throughput1"; record=mean, vector);
    
    gates:
        input in;
        output out;
        output out_queue;  // 👈 NUEVO: Necesario para el mensaje "puedes_enviar"
}
