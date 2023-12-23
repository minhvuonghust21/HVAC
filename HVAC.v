module hvac (clk,rst,t_cao,t_thap,keypad,den,bom,quat);

input clk,rst,t_cao,t_thap,keypad;
output reg den,bom,quat;

parameter [2:0] IDLE = 3'b000, Den=3'b001, Den1=3'b010,
                Thonggio=3'b011, Bom=3'b100, Bom1=3'b101;

reg [2:0] state_ht, state_next;

// khoi D2
always @(posedge clk or negedge rst) begin
    if (rst == 0) state_ht <= 3'b000;
    else state_ht <= state_next;
end

// khoi D1

always @(*) begin

    case (state_ht) 
                  IDLE : begin
                    if (keypad==0 && t_cao==0 && t_thap==0) state_next = IDLE;
                    else if (t_cao==1) state_next = Bom;
                    else if (t_thap==1) state_next = Den;
                    else if (keypad==1) state_next = Thonggio;
                  end
                  Bom : begin
                    if (t_cao==0) state_next = IDLE;
                    else if (keypad ==1) state_next = Bom1;
                  end
                  Den : begin
                    if (t_thap==0) state_next = IDLE;
                    else if (keypad ==1) state_next = Den1;
                  end
                  Thonggio : begin
                    if (keypad==0) state_next = IDLE;
                  end
                  Bom1 : begin
                    if (keypad==0) state_next = Bom;
                    else if (t_cao ==0 ) state_next = Thonggio;
                  end
                  Den1 : begin
                    if (keypad==0) state_next = Den;
                    else if (t_thap ==0 ) state_next = Thonggio;
                  end
                  default: state_next = IDLE;
    endcase
end

//khoi D3

always @(*) begin
    if (state_ht == IDLE) begin
        den =0;
        bom=0;
        quat=0;
    end
    else if (state_ht == Bom) begin
        den=0;
        bom=1;
        quat=0;
    end 
    else if (state_ht == Bom1) begin
        den =0;
        bom=1;
        quat=1;
    end
    else if (state_ht == Den) begin
        den=1;
        bom=0;
        quat=0;
    end
    else if (state_ht == Den1) begin
        den =1;
        bom=0;
        quat=1;
    end
    else if (state_ht == Thonggio) begin
        den =0;
        bom=0;
        quat=1;
    end else begin
        den =0;
        bom=0;
        quat=0;
    end
    
end
endmodule

module tb_hvac ();

reg clk,rst,t_cao,t_thap,keypad;
wire den,bom,quat;

hvac H (clk,rst,t_cao,t_thap,keypad,den,bom,quat);


initial begin
    clk=0;
    rst=1;
    #80 rst =0;
    #80 rst =1;
    t_cao =1;
    #80 keypad =1;
    #160 keypad =0;
    #70 t_cao =0;
    #200 keypad=1;
    
end

always #35 clk=~clk;
endmodule